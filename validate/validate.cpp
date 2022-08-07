#include <stdio.h>

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

#define CERTITUDE

auto scasrt_log = spdlog::stdout_color_st("scasrt_log");
#ifdef CONCAT_PASS
auto as_log =
    spdlog::basic_logger_mt("as_log", "../CONCAT_PASS_cpp.result", true);
#endif  // CONCAT_PASS

#ifdef CONCAT_FAIL
auto as_log =
    spdlog::basic_logger_mt("as_log", "../CONCAT_FAIL_cpp.result", true);
#endif  // CONCAT_FAIL

#ifdef ROSE_PASS
auto as_log =
    spdlog::basic_logger_mt("as_log", "../ROSE_PASS_cpp.result", true);
#endif  // ROSE_PASS

#ifdef ROSE_FAIL
auto as_log =
    spdlog::basic_logger_mt("as_log", "../ROSE_FAIL_cpp.result", true);
#endif  // ROSE_FAIL

#ifdef FELL_PASS
auto as_log =
    spdlog::basic_logger_mt("as_log", "../FELL_PASS_cpp.result", true);
#endif  // FELL_PASS

#ifdef FELL_FAIL
auto as_log =
    spdlog::basic_logger_mt("as_log", "../FELL_FAIL_cpp.result", true);
#endif  // FELL_FAIL

#ifdef PAST_PASS
auto as_log =
    spdlog::basic_logger_mt("as_log", "../PAST_PASS_cpp.result", true);
#endif  // PAST_PASS

#ifdef PAST_FAIL
auto as_log =
    spdlog::basic_logger_mt("as_log", "../PAST_FAIL_cpp.result", true);
#endif  // PAST_FAIL

#ifdef STABLE_PASS
auto as_log =
    spdlog::basic_logger_mt("as_log", "../STABLE_PASS_cpp.result", true);
#endif  // STABLE_PASS

#ifdef STABLE_FAIL
auto as_log =
    spdlog::basic_logger_mt("as_log", "../STABLE_FAIL_cpp.result", true);
#endif  // STABLE_FAIL

#ifdef INTERSECT_PASS
auto as_log =
    spdlog::basic_logger_mt("as_log", "../INTERSECT_PASS_cpp.result", true);
#endif  // INTERSECT_PASS

#ifdef INTERSECT_FAIL
auto as_log =
    spdlog::basic_logger_mt("as_log", "../INTERSECT_FAIL_cpp.result", true);
#endif  // INTERSECT_FAIL

#ifdef THROUGHOUT_PASS
auto as_log =
    spdlog::basic_logger_mt("as_log", "../THROUGHOUT_PASS_cpp.result", true);
#endif  // THROUGHOUT_PASS

#ifdef THROUGHOUT_FAIL
auto as_log =
    spdlog::basic_logger_mt("as_log", "../THROUGHOUT_FAIL_cpp.result", true);
#endif  // THROUGHOUT_FAIL

#ifdef WITHIN_PASS
auto as_log =
    spdlog::basic_logger_mt("as_log", "../WITHIN_PASS_cpp.result", true);
#endif  // WITHIN_PASS

#ifdef WITHIN_FAIL
auto as_log =
    spdlog::basic_logger_mt("as_log", "../WITHIN_FAIL_cpp.result", true);
#endif  // WITHIN_FAIL

#include "../src/Asrt.cpp"
#include "../src/Kernel.cpp"
#include "../src/Prop.cpp"
#include "../src/Tlm.cpp"
#include "../src/Types.cpp"

#define T(time) sc_core::sc_time(time, sc_core::SC_PS)

int sc_main(int argc, char* argv[]) { return 0; }

void addr_event(uint32_t set_val) {
  SPDLOG_LOGGER_INFO(as_log, "addr=0x{:08x}", set_val);

  tlm::tlm_generic_payload trans;
  unsigned char data[4] = {'A', 'B', 'C', 'D'};
  trans.set_address(set_val);
  trans.set_data_ptr((unsigned char*)&data);
  Scasrt::Kernel::GetInstance()->SnoopBTransportEnd(trans, "top", T(0));
}

int main() {
  as_log->set_pattern("%v");

  Scasrt::Kernel::GetInstance()->reset();

#ifdef CONCAT_PASS
  SPDLOG_LOGGER_INFO(as_log, "Test: CONCAT_PASS");

  // Assertion
  // A section
  auto a_strt_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_TRUE; });
  auto a_stop_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_FALSE; });
  auto a_eval_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP {
        if (tlm.address_ == 1) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        }
      });

  // B section
  auto b_strt_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_TRUE; });
  auto b_eval_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP {
        if (tlm.address_ == 2) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        }
      });

  // Assembly
  auto asrt_1_B = std::make_shared<Scasrt::Asrt>(
      "asrt_1_B", b_eval_prop_ptr, b_strt_prop_ptr, b_eval_prop_ptr,
      Scasrt::ASRT_PASS_ON_HOLD_STRONG);
  auto asrt_1_A = std::make_shared<Scasrt::Asrt>(
      "asrt_1_A", a_eval_prop_ptr, a_strt_prop_ptr, a_stop_prop_ptr, asrt_1_B);
  Scasrt::Kernel::GetInstance()->RegisterAsrt(asrt_1_A);

  Scasrt::Kernel::GetInstance()->Cleanup();

  // Stimulus
  addr_event(1);
  addr_event(2);
#endif  // CONCAT_PASS

#ifdef CONCAT_FAIL
  SPDLOG_LOGGER_INFO(as_log, "Test: CONCAT_FAIL");

  // Assertion
  // A section
  auto a_strt_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_TRUE; });
  auto a_stop_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_FALSE; });
  auto a_eval_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP {
        if (tlm.address_ == 1) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        }
      });

  // B section
  auto b_strt_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_TRUE; });
  auto b_eval_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP {
        if (tlm.address_ == 2) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        }
      });

  // Assembly
  auto asrt_1_B = std::make_shared<Scasrt::Asrt>(
      "asrt_1_B", b_eval_prop_ptr, b_strt_prop_ptr, b_eval_prop_ptr,
      Scasrt::ASRT_PASS_ON_HOLD_STRONG);
  auto asrt_1_A = std::make_shared<Scasrt::Asrt>(
      "asrt_1_A", a_eval_prop_ptr, a_strt_prop_ptr, a_stop_prop_ptr, asrt_1_B);
  Scasrt::Kernel::GetInstance()->RegisterAsrt(asrt_1_A);

  Scasrt::Kernel::GetInstance()->Cleanup();

  // Stimulus
  addr_event(1);
  addr_event(3);
#endif  // CONCAT_FAIL

#ifdef ROSE_PASS
  SPDLOG_LOGGER_INFO(as_log, "Test: ROSE_PASS");

  // Assertion
  // A section
  auto a_strt_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_TRUE; });
  auto a_stop_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_FALSE; });
  auto a_eval_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP {
        if (tlm.address_ == 0) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        }
      });

  // B section
  auto b_strt_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_TRUE; });
  auto b_eval_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP {
        if (history[0]->address_ == 0 && tlm.address_ == 1) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        }
      });

  // Assembly
  auto asrt_1_B = std::make_shared<Scasrt::Asrt>(
      "asrt_1_B", b_eval_prop_ptr, b_strt_prop_ptr, b_eval_prop_ptr,
      Scasrt::ASRT_PASS_ON_HOLD_STRONG);
  auto asrt_1_A = std::make_shared<Scasrt::Asrt>(
      "asrt_1_A", a_eval_prop_ptr, a_strt_prop_ptr, a_stop_prop_ptr, asrt_1_B);
  Scasrt::Kernel::GetInstance()->RegisterAsrt(asrt_1_A);

  Scasrt::Kernel::GetInstance()->Cleanup();

  // Stimulus
  addr_event(0);
  addr_event(1);
#endif  // ROSE_PASS

#ifdef ROSE_FAIL
  SPDLOG_LOGGER_INFO(as_log, "Test: ROSE_FAIL");

  // Assertion
  // A section
  auto a_strt_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_TRUE; });
  auto a_stop_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_TRUE; });
  auto a_eval_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP {
        if (tlm.address_ == 0) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        }
      });

  // B section
  auto b_strt_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_TRUE; });
  auto b_eval_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP {
        if (history[0]->address_ == 0 && tlm.address_ == 1) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        }
      });

  // Assembly
  auto asrt_1_B = std::make_shared<Scasrt::Asrt>(
      "asrt_1_B", b_eval_prop_ptr, b_strt_prop_ptr, b_eval_prop_ptr,
      Scasrt::ASRT_PASS_ON_HOLD_STRONG);
  auto asrt_1_A = std::make_shared<Scasrt::Asrt>(
      "asrt_1_A", a_eval_prop_ptr, a_strt_prop_ptr, a_stop_prop_ptr, asrt_1_B);
  Scasrt::Kernel::GetInstance()->RegisterAsrt(asrt_1_A);

  Scasrt::Kernel::GetInstance()->Cleanup();

  // Stimulus
  addr_event(0);
  addr_event(0);
#endif  // ROSE_FAIL

#ifdef FELL_PASS
  SPDLOG_LOGGER_INFO(as_log, "Test: FELL_PASS");

  // Assertion
  // A section
  auto a_strt_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_TRUE; });
  auto a_stop_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_TRUE; });
  auto a_eval_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP {
        if (tlm.address_ == 1) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        }
      });

  // B section
  auto b_strt_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_TRUE; });
  auto b_eval_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP {
        if (history[0]->address_ == 1 && tlm.address_ == 0) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        }
      });

  // Assembly
  auto asrt_1_B = std::make_shared<Scasrt::Asrt>(
      "asrt_1_B", b_eval_prop_ptr, b_strt_prop_ptr, b_eval_prop_ptr,
      Scasrt::ASRT_PASS_ON_HOLD_STRONG);
  auto asrt_1_A = std::make_shared<Scasrt::Asrt>(
      "asrt_1_A", a_eval_prop_ptr, a_strt_prop_ptr, a_stop_prop_ptr, asrt_1_B);
  Scasrt::Kernel::GetInstance()->RegisterAsrt(asrt_1_A);

  Scasrt::Kernel::GetInstance()->Cleanup();

  // Stimulus
  addr_event(1);
  addr_event(0);
#endif  // FELL_PASS

#ifdef FELL_FAIL
  SPDLOG_LOGGER_INFO(as_log, "Test: FELL_FAIL");

  // Assertion
  // A section
  auto a_strt_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_TRUE; });
  auto a_stop_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_TRUE; });
  auto a_eval_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP {
        if (tlm.address_ == 1) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        }
      });

  // B section
  auto b_strt_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_TRUE; });
  auto b_eval_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP {
        if (history[0]->address_ == 1 && tlm.address_ == 0) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        }
      });

  // Assembly
  auto asrt_1_B = std::make_shared<Scasrt::Asrt>(
      "asrt_1_B", b_eval_prop_ptr, b_strt_prop_ptr, b_eval_prop_ptr,
      Scasrt::ASRT_PASS_ON_HOLD_STRONG);
  auto asrt_1_A = std::make_shared<Scasrt::Asrt>(
      "asrt_1_A", a_eval_prop_ptr, a_strt_prop_ptr, a_stop_prop_ptr, asrt_1_B);
  Scasrt::Kernel::GetInstance()->RegisterAsrt(asrt_1_A);

  Scasrt::Kernel::GetInstance()->Cleanup();

  // Stimulus
  addr_event(1);
  addr_event(1);
#endif  // FELL_FAIL

#ifdef PAST_PASS
  SPDLOG_LOGGER_INFO(as_log, "Test: PAST_PASS");

  // Assertion
  // A section
  auto a_strt_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_TRUE; });
  auto a_stop_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_FALSE; });
  auto a_eval_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP {
        if (tlm.address_ == 1) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        }
      });

  // B section
  auto b_strt_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_TRUE; });
  auto b_eval_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP {
        if (history[0]->address_ == 1) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        }
      });

  // Assembly
  auto asrt_1_B = std::make_shared<Scasrt::Asrt>(
      "asrt_1_B", b_eval_prop_ptr, b_strt_prop_ptr, b_eval_prop_ptr,
      Scasrt::ASRT_PASS_ON_HOLD_STRONG);
  auto asrt_1_A = std::make_shared<Scasrt::Asrt>(
      "asrt_1_A", a_eval_prop_ptr, a_strt_prop_ptr, a_stop_prop_ptr, asrt_1_B);
  Scasrt::Kernel::GetInstance()->RegisterAsrt(asrt_1_A);

  Scasrt::Kernel::GetInstance()->Cleanup();

  // Stimulus
  addr_event(1);
  addr_event(0);
#endif  // PAST_PASS

#ifdef PAST_FAIL
  SPDLOG_LOGGER_INFO(as_log, "Test: PAST_FAIL");

  // Assertion
  // A section
  auto a_strt_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_TRUE; });
  auto a_stop_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_FALSE; });
  auto a_eval_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP {
        if (tlm.address_ == 1) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        }
      });

  // B section
  auto b_strt_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_TRUE; });
  auto b_eval_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP {
        if (history[0]->address_ == 0) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        }
      });

  // Assembly
  auto asrt_1_B = std::make_shared<Scasrt::Asrt>(
      "asrt_1_B", b_eval_prop_ptr, b_strt_prop_ptr, b_eval_prop_ptr,
      Scasrt::ASRT_PASS_ON_HOLD_STRONG);
  auto asrt_1_A = std::make_shared<Scasrt::Asrt>(
      "asrt_1_A", a_eval_prop_ptr, a_strt_prop_ptr, a_stop_prop_ptr, asrt_1_B);
  Scasrt::Kernel::GetInstance()->RegisterAsrt(asrt_1_A);

  Scasrt::Kernel::GetInstance()->Cleanup();

  // Stimulus
  addr_event(1);
  addr_event(0);
#endif  // PAST_FAIL

#ifdef STABLE_PASS
  SPDLOG_LOGGER_INFO(as_log, "Test: STABLE_PASS");

  // Assertion
  // A section
  auto a_strt_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_TRUE; });
  auto a_stop_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_TRUE; });
  auto a_eval_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP {
        if (tlm.address_ == 1) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        }
      });

  // B section
  auto b_strt_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_TRUE; });
  auto b_eval_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP {
        if (history[0]->address_ == tlm.address_) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        }
      });

  // Assembly
  auto asrt_1_B = std::make_shared<Scasrt::Asrt>(
      "asrt_1_B", b_eval_prop_ptr, b_strt_prop_ptr, b_eval_prop_ptr,
      Scasrt::ASRT_PASS_ON_HOLD_STRONG);
  auto asrt_1_A = std::make_shared<Scasrt::Asrt>(
      "asrt_1_A", a_eval_prop_ptr, a_strt_prop_ptr, a_stop_prop_ptr, asrt_1_B);
  Scasrt::Kernel::GetInstance()->RegisterAsrt(asrt_1_A);

  Scasrt::Kernel::GetInstance()->Cleanup();

  // Stimulus
  addr_event(1);
  addr_event(1);
#endif  // STABLE_PASS

#ifdef STABLE_FAIL
  SPDLOG_LOGGER_INFO(as_log, "Test: STABLE_FAIL");

  // Assertion
  // A section
  auto a_strt_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_TRUE; });
  auto a_stop_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_TRUE; });
  auto a_eval_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP {
        if (tlm.address_ == 1) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        }
      });

  // B section
  auto b_strt_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_TRUE; });
  auto b_eval_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP {
        if (history[0]->address_ == tlm.address_) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        }
      });

  // Assembly
  auto asrt_1_B = std::make_shared<Scasrt::Asrt>(
      "asrt_1_B", b_eval_prop_ptr, b_strt_prop_ptr, b_eval_prop_ptr,
      Scasrt::ASRT_PASS_ON_HOLD_STRONG);
  auto asrt_1_A = std::make_shared<Scasrt::Asrt>(
      "asrt_1_A", a_eval_prop_ptr, a_strt_prop_ptr, a_stop_prop_ptr, asrt_1_B);
  Scasrt::Kernel::GetInstance()->RegisterAsrt(asrt_1_A);

  Scasrt::Kernel::GetInstance()->Cleanup();

  // Stimulus
  addr_event(1);
  addr_event(0);
#endif  // STABLE_FAIL

#ifdef INTERSECT_PASS
  SPDLOG_LOGGER_INFO(as_log, "Test: INTERSECT_PASS");

  // Assertion
  // A section
  auto a_strt_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_TRUE; });
  auto a_stop_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_FALSE; });
  auto a_eval_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP {
        if (tlm.address_ == 1) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        }
      });

  // B section
  auto b_strt_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_TRUE; });
  auto b_stop_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_FALSE; });
  auto b_eval_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP {
        if (tlm.address_ == 3) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        }
      });

  // C section
  auto c_strt_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_TRUE; });
  auto c_eval_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP {
        if (tlm.address_ == 0) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        }
      });

  // Assembly
  auto asrt_1_C = std::make_shared<Scasrt::Asrt>(
      "asrt_1_C", c_eval_prop_ptr, c_strt_prop_ptr, c_eval_prop_ptr,
      Scasrt::ASRT_PASS_ON_HOLD_STRONG);
  auto asrt_1_B = std::make_shared<Scasrt::Asrt>(
      "asrt_1_B", b_eval_prop_ptr, b_strt_prop_ptr, b_eval_prop_ptr, asrt_1_C);
  auto asrt_1_A = std::make_shared<Scasrt::Asrt>(
      "asrt_1_A", a_eval_prop_ptr, a_strt_prop_ptr, a_stop_prop_ptr, asrt_1_B);
  Scasrt::Kernel::GetInstance()->RegisterAsrt(asrt_1_A);

  Scasrt::Kernel::GetInstance()->Cleanup();

  // Stimulus
  addr_event(1);
  addr_event(3);
  addr_event(0);
#endif  // INTERSECT_PASS

#ifdef INTERSECT_FAIL
  SPDLOG_LOGGER_INFO(as_log, "Test: INTERSECT_FAIL");

  // Assertion
  // A section
  auto a_strt_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_TRUE; });
  auto a_stop_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_TRUE; });
  auto a_eval_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP {
        if (tlm.address_ == 1) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        }
      });

  // B section
  auto b_strt_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_TRUE; });
  auto b_stop_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_FALSE; });
  auto b_eval_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP {
        if (tlm.address_ == 3) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        }
      });

  // C section
  auto c_strt_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_TRUE; });
  auto c_eval_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP {
        if (tlm.address_ == 0) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        }
      });

  // Assembly
  auto asrt_1_C = std::make_shared<Scasrt::Asrt>(
      "asrt_1_C", c_eval_prop_ptr, c_strt_prop_ptr, c_eval_prop_ptr,
      Scasrt::ASRT_PASS_ON_HOLD_STRONG);
  auto asrt_1_B = std::make_shared<Scasrt::Asrt>(
      "asrt_1_B", b_eval_prop_ptr, b_strt_prop_ptr, b_eval_prop_ptr, asrt_1_C);
  auto asrt_1_A = std::make_shared<Scasrt::Asrt>(
      "asrt_1_A", a_eval_prop_ptr, a_strt_prop_ptr, a_stop_prop_ptr, asrt_1_B);
  Scasrt::Kernel::GetInstance()->RegisterAsrt(asrt_1_A);

  Scasrt::Kernel::GetInstance()->Cleanup();

  // Stimulus
  addr_event(1);
  addr_event(3);
  addr_event(1);
#endif  // INTERSECT_FAIL

#ifdef THROUGHOUT_PASS
  SPDLOG_LOGGER_INFO(as_log, "Test: THROUGHOUT_PASS");

  // Assertion
  // A section
  auto a_strt_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_TRUE; });
  auto a_stop_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_FALSE; });
  auto a_eval_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP {
        if (tlm.address_ == 2) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        }
      });

  // B section
  auto b_strt_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_TRUE; });
  auto b_eval_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP {
        if (tlm.address_ == 3) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        }
      });

  // Assembly
  auto asrt_1_B = std::make_shared<Scasrt::Asrt>(
      "asrt_1_B", b_eval_prop_ptr, b_strt_prop_ptr, b_eval_prop_ptr,
      Scasrt::ASRT_PASS_ON_HOLD_STRONG);
  auto asrt_1_A = std::make_shared<Scasrt::Asrt>(
      "asrt_1_A", a_eval_prop_ptr, a_strt_prop_ptr, a_stop_prop_ptr, asrt_1_B);
  Scasrt::Kernel::GetInstance()->RegisterAsrt(asrt_1_A);

  Scasrt::Kernel::GetInstance()->Cleanup();

  // Stimulus
  addr_event(2);
  addr_event(3);
#endif  // THROUGHOUT_PASS

#ifdef THROUGHOUT_FAIL
  SPDLOG_LOGGER_INFO(as_log, "Test: THROUGHOUT_FAIL");

  // Assertion
  // A section
  auto a_strt_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_TRUE; });
  auto a_stop_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_FALSE; });
  auto a_eval_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP {
        if (tlm.address_ == 2) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        }
      });

  // B section
  auto b_strt_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_TRUE; });
  auto b_eval_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP {
        if (tlm.address_ == 3) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        }
      });

  // Assembly
  auto asrt_1_B = std::make_shared<Scasrt::Asrt>(
      "asrt_1_B", b_eval_prop_ptr, b_strt_prop_ptr, b_eval_prop_ptr,
      Scasrt::ASRT_PASS_ON_HOLD_STRONG);
  auto asrt_1_A = std::make_shared<Scasrt::Asrt>(
      "asrt_1_A", a_eval_prop_ptr, a_strt_prop_ptr, a_stop_prop_ptr, asrt_1_B);
  Scasrt::Kernel::GetInstance()->RegisterAsrt(asrt_1_A);

  Scasrt::Kernel::GetInstance()->Cleanup();

  // Stimulus
  addr_event(2);
  addr_event(6);
#endif  // THROUGHOUT_FAIL

#ifdef WITHIN_PASS
  SPDLOG_LOGGER_INFO(as_log, "Test: WITHIN_PASS");

  // Assertion
  // A section
  auto a_strt_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP {
        if ((tlm.address_ & 1) == 1) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        }
      });
  auto a_stop_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_FALSE; });
  auto a_eval_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP {
        if ((tlm.address_ & 2) == 2) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        }
      });

  // B section
  auto b_strt_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_TRUE; });
  auto b_stop_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP {
        if ((tlm.address_ & 1) == 0) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        }
      });
  auto b_eval_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP {
        if ((tlm.address_ & 2) == 0) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        }
      });

  // Assembly
  auto asrt_1_B = std::make_shared<Scasrt::Asrt>(
      "asrt_1_B", b_eval_prop_ptr, b_strt_prop_ptr, b_stop_prop_ptr,
      Scasrt::ASRT_PASS_ON_HOLD_STRONG);
  auto asrt_1_A = std::make_shared<Scasrt::Asrt>(
      "asrt_1_A", a_eval_prop_ptr, a_strt_prop_ptr, a_eval_prop_ptr, asrt_1_B);
  Scasrt::Kernel::GetInstance()->RegisterAsrt(asrt_1_A);

  Scasrt::Kernel::GetInstance()->Cleanup();

  // Stimulus
  addr_event(0b0001);
  addr_event(0b0011);
  addr_event(0b0101);
  addr_event(0b1010);
#endif  // WITHIN_PASS

#ifdef WITHIN_FAIL
  SPDLOG_LOGGER_INFO(as_log, "Test: WITHIN_FAIL");

  // Assertion
  // A section
  auto a_strt_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP {
        if ((tlm.address_ & 1) == 1) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        }
      });
  auto a_stop_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_FALSE; });
  auto a_eval_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP {
        if ((tlm.address_ & 2) == 2) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        }
      });

  // B section
  auto b_strt_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP { return Scasrt::PROP_TRUE; });
  auto b_stop_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP {
        if ((tlm.address_ & 1) == 0) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        }
      });
  auto b_eval_prop_ptr = std::make_shared<Scasrt::Prop>(
      "top", L_PROP {
        if ((tlm.address_ & 2) == 0) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        }
      });

  // Assembly
  auto asrt_1_B = std::make_shared<Scasrt::Asrt>(
      "asrt_1_B", b_eval_prop_ptr, b_strt_prop_ptr, b_stop_prop_ptr,
      Scasrt::ASRT_PASS_ON_HOLD_STRONG);
  auto asrt_1_A = std::make_shared<Scasrt::Asrt>(
      "asrt_1_A", a_eval_prop_ptr, a_strt_prop_ptr, a_eval_prop_ptr, asrt_1_B);
  Scasrt::Kernel::GetInstance()->RegisterAsrt(asrt_1_A);

  Scasrt::Kernel::GetInstance()->Cleanup();

  // Stimulus
  addr_event(0b0001);
  addr_event(0b0001);
  addr_event(0b0101);
  addr_event(0b1010);
#endif  // WITHIN_FAIL

  Scasrt::Kernel::GetInstance()->RunEot();

  return 0;
}
