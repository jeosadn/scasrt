#include "Kernel.h"

#include "Tlm.h"

namespace Scasrt {
Kernel* Kernel::pinstance_{nullptr};
int Kernel::next_id_(0);
int Kernel::snoop_count_(0);
int Kernel::eval_count_(0);
int Kernel::pass_count_(0);
int Kernel::fail_count_(0);
bool Kernel::first_cleanup_(true);
bool Kernel::print_asrts_(false);

std::vector<std::shared_ptr<Asrt> > Kernel::active_asrts_;
std::vector<std::shared_ptr<Asrt> > Kernel::inactive_asrts_;
std::vector<std::shared_ptr<Asrt> > Kernel::next_asrts_;

Kernel* Kernel::GetInstance() {
  if (nullptr == pinstance_) {
    pinstance_ = new Kernel();
  }

  return pinstance_;
}

void Kernel::reset() {
  SPDLOG_LOGGER_DEBUG(spdlog::get("scasrt_log"), "Resetting...");
  next_id_ = 0;
  snoop_count_ = 0;
  eval_count_ = 0;
  pass_count_ = 0;
  fail_count_ = 0;
  first_cleanup_ = true;
  print_asrts_ = false;
  active_asrts_.clear();
  inactive_asrts_.clear();
  next_asrts_.clear();
}

void Kernel::RegisterAsrt(std::shared_ptr<Asrt> asrt) {
  asrt->SetId(++next_id_);
  next_asrts_.emplace_back(std::move(asrt));
  SPDLOG_LOGGER_DEBUG(
      spdlog::get("scasrt_log"), "Registered Asrt {}, history: {}",
      next_asrts_.back()->Name(), next_asrts_.back()->history_.size());
}

void Kernel::Cleanup() {
  if (first_cleanup_) {
    first_cleanup_ = false;
    SPDLOG_LOGGER_DEBUG(
        spdlog::get("scasrt_log"),
        "For this run, the kernel is starting with {} registered assertions",
        next_asrts_.size());
  }
  SPDLOG_LOGGER_DEBUG(spdlog::get("scasrt_log"), "Start of cleanup in Kernel");

  SPDLOG_LOGGER_DEBUG(spdlog::get("scasrt_log"),
                      "Looking for spawned assertions");
  for (auto& asrt : active_asrts_) {
    if (asrt->spawn_next_asrt_) {
      SPDLOG_LOGGER_DEBUG(spdlog::get("scasrt_log"), "{} is spawning",
                          asrt->Name());
      RegisterAsrt(asrt->GenNextAsrt());
    }
  }

  SPDLOG_LOGGER_DEBUG(spdlog::get("scasrt_log"),
                      "Looking for expired assertions");
  for (auto& asrt : active_asrts_) {
    if (asrt->state_ == ASRT_EXPIRED && asrt->spawn_count_ == 0) {
      SPDLOG_LOGGER_DEBUG(spdlog::get("scasrt_log"), "{} is expired",
                          asrt->Name());
      inactive_asrts_.emplace_back(std::move(asrt));
    }
  }

  // Clean up null pointers or expired spawners from active_asrts_
  active_asrts_.erase(
      std::remove_if(active_asrts_.begin(), active_asrts_.end(),
                     [](std::shared_ptr<Asrt> const& asrt) {
                       if (nullptr == asrt) {
                         return true;
                       } else if (asrt->state_ == ASRT_EXPIRED &&
                                  asrt->spawn_count_ > 0) {
                         return true;
                       } else {
                         return false;
                       }
                     }),
      active_asrts_.end());

  // Move all disabled assertions from next_asrts_ to active_asrts_
  for (auto& asrt : next_asrts_) {
    if (asrt->state_ == ASRT_DISABLED) {
      SPDLOG_LOGGER_DEBUG(spdlog::get("scasrt_log"),
                          "{} is disabled, moving to active_asrts_",
                          asrt->Name());
      active_asrts_.emplace_back(std::move(asrt));
    } else if (asrt->state_ == ASRT_MATCHED) {
      SPDLOG_LOGGER_DEBUG(spdlog::get("scasrt_log"),
                          "{} is matched, moving to inactive_asrts_",
                          asrt->Name());
      inactive_asrts_.emplace_back(std::move(asrt));
    } else {
      SPDLOG_LOGGER_ERROR(
          spdlog::get("scasrt_log"),
          "Unexpected state %s in %s when processing next_asrts_", asrt->Name(),
          asrt->state_);
      exit(1);
    }
  }

  // Clean up inactive asserts
  for (auto& asrt : inactive_asrts_) {
    auto status = asrt->GetStatus();
    auto criteria = asrt->GetCriteria();
    if (status == ASRT_HOLDS_STRONGLY || status == ASRT_HOLDS_SOFTLY ||
        status == ASRT_DOESNT_HOLD) {
      eval_count_++;
      if (asrt->Passes(status, criteria)) {
        pass_count_++;
        if (print_asrts_) {
          SPDLOG_LOGGER_INFO(spdlog::get("scasrt_log"),
                             "PASS: {}: {}, criteria={}", asrt->Name(),
                             asrt_status_to_name[status],
                             asrt_pass_to_name[criteria]);
          for (auto& tlm : asrt->history_) {
            SPDLOG_LOGGER_INFO(
                spdlog::get("scasrt_log"),
                "History: probe={}, cmd=0x{:X}, address=0x{:08X}, "
                "response=0x{:X}, dmi={}, "
                "length=0x{:X}, data[4:0]=0x{:08X}",
                tlm->observation_point_, tlm->command_, tlm->address_,
                tlm->response_status_, tlm->dmi_, tlm->length_, tlm->data_i_);
          }
#ifdef CERTITUDE
          SPDLOG_LOGGER_INFO(spdlog::get("as_log"), "PASS");
#endif
        }
      } else {
        fail_count_++;
        if (print_asrts_) {
          SPDLOG_LOGGER_ERROR(spdlog::get("scasrt_log"),
                              "FAIL: {}: {}, criteria={}", asrt->Name(),
                              asrt_status_to_name[status],
                              asrt_pass_to_name[criteria]);
          for (auto& tlm : asrt->history_) {
            SPDLOG_LOGGER_INFO(
                spdlog::get("scasrt_log"),
                "History: probe={}, cmd=0x{:X}, address=0x{:08X}, "
                "response=0x{:X}, dmi={}, "
                "length=0x{:X}, data[4:0]=0x{:08X}",
                tlm->observation_point_, tlm->command_, tlm->address_,
                tlm->response_status_, tlm->dmi_, tlm->length_, tlm->data_i_);
          }
#ifdef CERTITUDE
          SPDLOG_LOGGER_INFO(spdlog::get("as_log"), "FAIL");
#endif
        }
      }
    }
  }
  inactive_asrts_.clear();

  // Clean up all contents of next_asrts_
  next_asrts_.clear();
}

void Kernel::SnoopBTransportBgn(const tlm::tlm_generic_payload& trans,
                                const std::string& probe,
                                const sc_core::sc_time& time) {
  SPDLOG_LOGGER_DEBUG(
      spdlog::get("scasrt_log"),
      "Start of b_transport in {} for tran({}): cmd=0x{:X}, "
      "address=0x{:08X}, response=0x{:X}, dmi={}, length=0x{:X}, "
      "data[4:0]=0x{:02X}{:02X}{:02X}{:02X}",
      probe, (void*)&trans, trans.get_command(), trans.get_address(),
      trans.get_response_status(), trans.is_dmi_allowed(),
      trans.get_data_length(), trans.get_data_ptr()[3], trans.get_data_ptr()[2],
      trans.get_data_ptr()[1], trans.get_data_ptr()[0]);

  snoop_count_++;

  auto tlm = std::make_shared<Scasrt::Tlm>(trans, probe, time, B_TRANSPORT_BGN);
  for (auto& asrt : active_asrts_) {
    asrt->Main(tlm);
  }

  Scasrt::Kernel::GetInstance()->Cleanup();
}

void Kernel::SnoopBTransportEnd(const tlm::tlm_generic_payload& trans,
                                const std::string& probe,
                                const sc_core::sc_time& time) {
  SPDLOG_LOGGER_DEBUG(
      spdlog::get("scasrt_log"),
      "End of   b_transport in {} for tran({}): cmd=0x{:X}, "
      "address=0x{:08X}, response=0x{:X}, dmi={}, length=0x{:X}, "
      "data[4:0]=0x{:02X}{:02X}{:02X}{:02X}",
      probe, (void*)&trans, trans.get_command(), trans.get_address(),
      trans.get_response_status(), trans.is_dmi_allowed(),
      trans.get_data_length(), trans.get_data_ptr()[3], trans.get_data_ptr()[2],
      trans.get_data_ptr()[1], trans.get_data_ptr()[0]);

  snoop_count_++;

  auto tlm = std::make_shared<Scasrt::Tlm>(trans, probe, time, B_TRANSPORT_END);
  for (auto& asrt : active_asrts_) {
    asrt->Main(tlm);
  }

  Scasrt::Kernel::GetInstance()->Cleanup();
}

void Kernel::SnoopNbTransportFwBgn(const tlm::tlm_generic_payload& trans,
                                   const std::string& probe,
                                   const sc_core::sc_time& time,
                                   const tlm::tlm_phase& phase) {
  SPDLOG_LOGGER_DEBUG(spdlog::get("scasrt_log"),
                      "Start of nb_transport_fw in {} for tran({}): "
                      "cmd=0x{:X}, address=0x{:08X}, "
                      "response=0x{:X}, phase={}, dmi={}, length=0x{:X}, "
                      "data[4:0]={:02X}{:02X}{:02X}{:02X}",
                      probe, (void*)&trans, trans.get_command(),
                      trans.get_address(), trans.get_response_status(), phase,
                      trans.is_dmi_allowed(), trans.get_data_length(),
                      trans.get_data_ptr()[3], trans.get_data_ptr()[2],
                      trans.get_data_ptr()[1], trans.get_data_ptr()[0]);

  snoop_count_++;

  auto tlm = std::make_shared<Scasrt::Tlm>(trans, probe, time,
                                           NB_TRANSPORT_FW_BGN, phase);
  for (auto& asrt : active_asrts_) {
    asrt->Main(tlm);
  }

  Scasrt::Kernel::GetInstance()->Cleanup();
}

void Kernel::SnoopNbTransportFwEnd(const tlm::tlm_generic_payload& trans,
                                   const std::string& probe,
                                   const sc_core::sc_time& time,
                                   const tlm::tlm_phase& phase,
                                   const tlm::tlm_sync_enum& status) {
  SPDLOG_LOGGER_DEBUG(spdlog::get("scasrt_log"),
                      "End of   nb_transport_fw in {} for tran({}): "
                      "cmd=0x{:X}, address=0x{:X}, "
                      "response=0x{:X}, phase={}, dmi={}, length=0x{:X}, "
                      "data[4:0]={:02X}{:02X}{:02X}{:02X}",
                      probe, (void*)&trans, trans.get_command(),
                      trans.get_address(), trans.get_response_status(), phase,
                      trans.is_dmi_allowed(), trans.get_data_length(),
                      trans.get_data_ptr()[3], trans.get_data_ptr()[2],
                      trans.get_data_ptr()[1], trans.get_data_ptr()[0]);

  snoop_count_++;

  auto tlm = std::make_shared<Scasrt::Tlm>(trans, probe, time,
                                           NB_TRANSPORT_FW_END, phase, status);
  for (auto& asrt : active_asrts_) {
    asrt->Main(tlm);
  }

  Scasrt::Kernel::GetInstance()->Cleanup();
}

void Kernel::SnoopNbTransportBwBgn(const tlm::tlm_generic_payload& trans,
                                   const std::string& probe,
                                   const sc_core::sc_time& time,
                                   const tlm::tlm_phase& phase) {
  SPDLOG_LOGGER_DEBUG(spdlog::get("scasrt_log"),
                      "Start of nb_transport_bw in {} for tran({}): "
                      "cmd=0x{:X}, address=0x{:X}, "
                      "response=0x{:X}, phase={}, dmi={}, length=0x{:X}, "
                      "data[4:0]={:02X}{:02X}{:02X}{:02X}",
                      probe, (void*)&trans, trans.get_command(),
                      trans.get_address(), trans.get_response_status(), phase,
                      trans.is_dmi_allowed(), trans.get_data_length(),
                      trans.get_data_ptr()[3], trans.get_data_ptr()[2],
                      trans.get_data_ptr()[1], trans.get_data_ptr()[0]);

  snoop_count_++;

  auto tlm = std::make_shared<Scasrt::Tlm>(trans, probe, time,
                                           NB_TRANSPORT_BW_BGN, phase);
  for (auto& asrt : active_asrts_) {
    asrt->Main(tlm);
  }

  Scasrt::Kernel::GetInstance()->Cleanup();
}

void Kernel::SnoopNbTransportBwEnd(const tlm::tlm_generic_payload& trans,
                                   const std::string& probe,
                                   const sc_core::sc_time& time,
                                   const tlm::tlm_phase& phase,
                                   const tlm::tlm_sync_enum& status) {
  SPDLOG_LOGGER_DEBUG(spdlog::get("scasrt_log"),
                      "End of   nb_transport_bw in {} for tran({}): "
                      "cmd=0x{:X}, address=0x{:X}, "
                      "response=0x{:X}, phase={}, dmi={}, length=0x{:X}, "
                      "data[4:0]={:02X}{:02X}{:02X}{:02X}",
                      probe, (void*)&trans, trans.get_command(),
                      trans.get_address(), trans.get_response_status(), phase,
                      trans.is_dmi_allowed(), trans.get_data_length(),
                      trans.get_data_ptr()[3], trans.get_data_ptr()[2],
                      trans.get_data_ptr()[1], trans.get_data_ptr()[0]);

  snoop_count_++;

  auto tlm = std::make_shared<Scasrt::Tlm>(trans, probe, time,
                                           NB_TRANSPORT_BW_END, phase, status);
  for (auto& asrt : active_asrts_) {
    asrt->Main(tlm);
  }

  Scasrt::Kernel::GetInstance()->Cleanup();
}

void Kernel::RunEot() {
  SPDLOG_LOGGER_DEBUG(spdlog::get("scasrt_log"), "Kernel reached EOT");
  for (auto& asrt : active_asrts_) {
    auto status = asrt->GetStatus();
    auto criteria = asrt->GetCriteria();
    if (status == ASRT_HOLDS_STRONGLY || status == ASRT_HOLDS_SOFTLY ||
        status == ASRT_DOESNT_HOLD) {
      eval_count_++;
      if (asrt->Passes(status, criteria)) {
        pass_count_++;
        if (print_asrts_) {
          SPDLOG_LOGGER_INFO(spdlog::get("scasrt_log"),
                             "PASS: {}: {}, criteria={}", asrt->Name(),
                             asrt_status_to_name[status],
                             asrt_pass_to_name[criteria]);
          for (auto& tlm : asrt->history_) {
            SPDLOG_LOGGER_INFO(
                spdlog::get("scasrt_log"),
                "History: probe={}, cmd=0x{:X}, address=0x{:08X}, "
                "response=0x{:X}, dmi={}, "
                "length=0x{:X}, data[4:0]=0x{:08X}",
                tlm->observation_point_, tlm->command_, tlm->address_,
                tlm->response_status_, tlm->dmi_, tlm->length_, tlm->data_i_);
          }
#ifdef CERTITUDE
          SPDLOG_LOGGER_INFO(spdlog::get("as_log"), "PASS");
#endif
        }
      } else {
        fail_count_++;
        if (print_asrts_) {
          SPDLOG_LOGGER_ERROR(spdlog::get("scasrt_log"),
                              "FAIL: {}: {}, criteria={}", asrt->Name(),
                              asrt_status_to_name[status],
                              asrt_pass_to_name[criteria]);
          for (auto& tlm : asrt->history_) {
            SPDLOG_LOGGER_INFO(
                spdlog::get("scasrt_log"),
                "History: probe={}, cmd=0x{:X}, address=0x{:08X}, "
                "response=0x{:X}, dmi={}, "
                "length=0x{:X}, data[4:0]=0x{:08X}",
                tlm->observation_point_, tlm->command_, tlm->address_,
                tlm->response_status_, tlm->dmi_, tlm->length_, tlm->data_i_);
          }
#ifdef CERTITUDE
          SPDLOG_LOGGER_INFO(spdlog::get("as_log"), "FAIL");
#endif
        }
      }
    }
  }

  if (inactive_asrts_.size() != 0) {
    SPDLOG_LOGGER_ERROR(spdlog::get("scasrt_log"),
                        "inactive_asrts_.size() == {}", inactive_asrts_.size());
  }

  if (next_asrts_.size() != 0) {
    SPDLOG_LOGGER_ERROR(spdlog::get("scasrt_log"), "next_asrts_.size() == {}",
                        next_asrts_.size());
  }

  SPDLOG_LOGGER_INFO(
      spdlog::get("scasrt_log"),
      "For this run, the kernel snooped {} times, generated {} assertion "
      "atoms, evaluated {} assertions, passed {} assertions, failed {} "
      "assertions, and had {} active assertions at the EOT",
      snoop_count_, next_id_, eval_count_, pass_count_, fail_count_,
      active_asrts_.size());
}

std::string Kernel::ToString() {
  std::string result = fmt::format("Kernel status:");
  result = fmt::format("{}\nactive_asrts_:", result);
  for (auto& asrt : active_asrts_) {
    result = fmt::format("{}\n    {}", result, asrt->ToString());
  }

  result = fmt::format("{}\ninactive_asrts_:", result);
  for (auto& asrt : inactive_asrts_) {
    result = fmt::format("{}\n    {}", result, asrt->ToString());
  }

  result = fmt::format("{}\nnext_asrts_:", result);
  for (auto& asrt : next_asrts_) {
    result = fmt::format("{}\n    {}", result, asrt->ToString());
  }

  return result;
}
}  // namespace Scasrt

std::ostream& operator<<(std::ostream& os, const Scasrt::Kernel& value) {
  os << value.ToString();
  return os;
}
