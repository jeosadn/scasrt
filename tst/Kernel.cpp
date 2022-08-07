#include "../inc/Probe.h"
#include "../tst/Utils.h"

// ----------------------------------------------------------------------------
//  Unit tests
// ----------------------------------------------------------------------------
TEST_CASE("Scasrt::Kernel::GetInstance") {
  setup_testcase();

  SPDLOG_LOGGER_INFO(spdlog::get("scasrt_test_log"),
                     "Scasrt::Kernel::GetInstance");

  SPDLOG_LOGGER_INFO(spdlog::get("scasrt_test_log"),
                     Scasrt::Kernel::GetInstance()->ToString());

  CHECK(Scasrt::Kernel::GetInstance()->next_id_ == 0);
  CHECK(Scasrt::Kernel::GetInstance()->snoop_count_ == 0);
  CHECK(Scasrt::Kernel::GetInstance()->eval_count_ == 0);
  CHECK(Scasrt::Kernel::GetInstance()->pass_count_ == 0);
  CHECK(Scasrt::Kernel::GetInstance()->fail_count_ == 0);
  CHECK(Scasrt::Kernel::GetInstance()->active_asrts_.size() == 0);
  CHECK(Scasrt::Kernel::GetInstance()->inactive_asrts_.size() == 0);
  CHECK(Scasrt::Kernel::GetInstance()->next_asrts_.size() == 0);
  CHECK(Scasrt::Kernel::GetInstance()->first_cleanup_);
  CHECK(!Scasrt::Kernel::GetInstance()->print_asrts_);
}

TEST_CASE("Scasrt::Kernel::RegisterAsrt") {
  setup_testcase();

  SPDLOG_LOGGER_INFO(spdlog::get("scasrt_test_log"),
                     "Scasrt::Kernel::RegisterAsrt");

  SPDLOG_LOGGER_INFO(spdlog::get("scasrt_test_log"),
                     Scasrt::Kernel::GetInstance()->ToString());

  auto start_prop_ptr = TIME_PROP(PROBE_Y, T(0));
  auto stop_prop_ptr = TIME_PROP(PROBE_Y, T(1));
  auto eval_prop_ptr = SIMPLE_PROP(PROBE_Y, ADDR_A);

  auto leaf_asrt =
      std::make_shared<Scasrt::Asrt>("leaf_asrt", eval_prop_ptr, start_prop_ptr,
                                     stop_prop_ptr, Scasrt::ASRT_PASS_ON_HOLD);

  Scasrt::Kernel::GetInstance()->RegisterAsrt(leaf_asrt);
  CHECK(Scasrt::Kernel::GetInstance()->active_asrts_.size() == 0);
  CHECK(Scasrt::Kernel::GetInstance()->inactive_asrts_.size() == 0);
  CHECK(Scasrt::Kernel::GetInstance()->next_asrts_.size() == 1);
}

TEST_CASE("Scasrt::Kernel::Cleanup") {
  setup_testcase();

  SPDLOG_LOGGER_INFO(spdlog::get("scasrt_test_log"), "Scasrt::Kernel::Cleanup");

  SPDLOG_LOGGER_INFO(spdlog::get("scasrt_test_log"),
                     Scasrt::Kernel::GetInstance()->ToString());

  auto start_prop_ptr = TIME_PROP(PROBE_Y, T(0));
  auto stop_prop_ptr = TIME_PROP(PROBE_Y, T(1));
  auto eval_prop_ptr = SIMPLE_PROP(PROBE_Y, ADDR_A);

  auto leaf_asrt =
      std::make_shared<Scasrt::Asrt>("leaf_asrt", eval_prop_ptr, start_prop_ptr,
                                     stop_prop_ptr, Scasrt::ASRT_PASS_ON_HOLD);

  Scasrt::Kernel::GetInstance()->RegisterAsrt(leaf_asrt);
  CHECK(Scasrt::Kernel::GetInstance()->active_asrts_.size() == 0);
  CHECK(Scasrt::Kernel::GetInstance()->inactive_asrts_.size() == 0);
  CHECK(Scasrt::Kernel::GetInstance()->next_asrts_.size() == 1);

  Scasrt::Kernel::GetInstance()->Cleanup();
  CHECK(Scasrt::Kernel::GetInstance()->active_asrts_.size() == 1);
  CHECK(Scasrt::Kernel::GetInstance()->inactive_asrts_.size() == 0);
  CHECK(Scasrt::Kernel::GetInstance()->next_asrts_.size() == 0);
  CHECK(!Scasrt::Kernel::GetInstance()->first_cleanup_);
}

TEST_CASE("Scasrt::Kernel::SnoopBTransportBgn") {
  setup_testcase();

  SPDLOG_LOGGER_INFO(spdlog::get("scasrt_test_log"),
                     "Scasrt::Kernel::SnoopBTransportBgn");

  SPDLOG_LOGGER_INFO(spdlog::get("scasrt_test_log"),
                     Scasrt::Kernel::GetInstance()->ToString());

  auto start_prop_ptr = SIMPLE_PROP(PROBE_Y, ADDR_A);
  auto stop_prop_ptr = SIMPLE_PROP(PROBE_Y, ADDR_A);
  auto eval_prop_ptr = SIMPLE_PROP(PROBE_Y, ADDR_A);

  auto leaf_asrt =
      std::make_shared<Scasrt::Asrt>("leaf_asrt", eval_prop_ptr, start_prop_ptr,
                                     stop_prop_ptr, Scasrt::ASRT_PASS_ON_HOLD);

  Scasrt::Kernel::GetInstance()->RegisterAsrt(leaf_asrt);
  CHECK(Scasrt::Kernel::GetInstance()->active_asrts_.size() == 0);
  CHECK(Scasrt::Kernel::GetInstance()->inactive_asrts_.size() == 0);
  CHECK(Scasrt::Kernel::GetInstance()->next_asrts_.size() == 1);

  Scasrt::Kernel::GetInstance()->Cleanup();
  CHECK(Scasrt::Kernel::GetInstance()->active_asrts_.size() == 1);
  CHECK(Scasrt::Kernel::GetInstance()->inactive_asrts_.size() == 0);
  CHECK(Scasrt::Kernel::GetInstance()->next_asrts_.size() == 0);
  CHECK(!Scasrt::Kernel::GetInstance()->first_cleanup_);

  tlm::tlm_generic_payload trans;
  trans.set_address(ADDR_A);
  unsigned char data[4] = {'A', 'B', 'C', 'D'};
  trans.set_data_ptr((unsigned char*)&data);

  Scasrt::Kernel::GetInstance()->SnoopBTransportBgn(trans, PROBE_Y, T(0));
  CHECK(leaf_asrt->spawn_count_ == 1);
  CHECK(leaf_asrt->latest_tlm_->snoop_phase_ == Scasrt::B_TRANSPORT_BGN);
  CHECK(Scasrt::Kernel::GetInstance()->snoop_count_ == 1);
  CHECK(Scasrt::Kernel::GetInstance()->eval_count_ == 1);
  CHECK(Scasrt::Kernel::GetInstance()->pass_count_ == 1);
  CHECK(Scasrt::Kernel::GetInstance()->fail_count_ == 0);
  CHECK(Scasrt::Kernel::GetInstance()->active_asrts_.size() == 0);
  CHECK(Scasrt::Kernel::GetInstance()->inactive_asrts_.size() == 0);
  CHECK(Scasrt::Kernel::GetInstance()->next_asrts_.size() == 0);
}

TEST_CASE("Scasrt::Kernel::SnoopBTransportEnd") {
  setup_testcase();

  SPDLOG_LOGGER_INFO(spdlog::get("scasrt_test_log"),
                     "Scasrt::Kernel::SnoopBTransportEnd");

  SPDLOG_LOGGER_INFO(spdlog::get("scasrt_test_log"),
                     Scasrt::Kernel::GetInstance()->ToString());

  auto start_prop_ptr = SIMPLE_PROP(PROBE_Y, ADDR_A);
  auto stop_prop_ptr = SIMPLE_PROP(PROBE_Y, ADDR_A);
  auto eval_prop_ptr = SIMPLE_PROP(PROBE_Y, ADDR_A);

  auto leaf_asrt =
      std::make_shared<Scasrt::Asrt>("leaf_asrt", eval_prop_ptr, start_prop_ptr,
                                     stop_prop_ptr, Scasrt::ASRT_PASS_ON_HOLD);

  Scasrt::Kernel::GetInstance()->RegisterAsrt(leaf_asrt);
  CHECK(Scasrt::Kernel::GetInstance()->active_asrts_.size() == 0);
  CHECK(Scasrt::Kernel::GetInstance()->inactive_asrts_.size() == 0);
  CHECK(Scasrt::Kernel::GetInstance()->next_asrts_.size() == 1);

  Scasrt::Kernel::GetInstance()->Cleanup();
  CHECK(Scasrt::Kernel::GetInstance()->active_asrts_.size() == 1);
  CHECK(Scasrt::Kernel::GetInstance()->inactive_asrts_.size() == 0);
  CHECK(Scasrt::Kernel::GetInstance()->next_asrts_.size() == 0);
  CHECK(!Scasrt::Kernel::GetInstance()->first_cleanup_);

  tlm::tlm_generic_payload trans;
  trans.set_address(ADDR_A);
  unsigned char data[4] = {'A', 'B', 'C', 'D'};
  trans.set_data_ptr((unsigned char*)&data);

  Scasrt::Kernel::GetInstance()->SnoopBTransportEnd(trans, PROBE_Y, T(0));
  CHECK(leaf_asrt->spawn_count_ == 1);
  CHECK(leaf_asrt->latest_tlm_->snoop_phase_ == Scasrt::B_TRANSPORT_END);
  CHECK(Scasrt::Kernel::GetInstance()->snoop_count_ == 1);
  CHECK(Scasrt::Kernel::GetInstance()->eval_count_ == 1);
  CHECK(Scasrt::Kernel::GetInstance()->pass_count_ == 1);
  CHECK(Scasrt::Kernel::GetInstance()->fail_count_ == 0);
  CHECK(Scasrt::Kernel::GetInstance()->active_asrts_.size() == 0);
  CHECK(Scasrt::Kernel::GetInstance()->inactive_asrts_.size() == 0);
  CHECK(Scasrt::Kernel::GetInstance()->next_asrts_.size() == 0);
}

TEST_CASE("Scasrt::Kernel::SnoopNbTransportFwBgn") {
  setup_testcase();

  SPDLOG_LOGGER_INFO(spdlog::get("scasrt_test_log"),
                     "Scasrt::Kernel::SnoopNbTransportFwBgn");

  SPDLOG_LOGGER_INFO(spdlog::get("scasrt_test_log"),
                     Scasrt::Kernel::GetInstance()->ToString());

  auto start_prop_ptr = SIMPLE_PROP(PROBE_Y, ADDR_A);
  auto stop_prop_ptr = SIMPLE_PROP(PROBE_Y, ADDR_A);
  auto eval_prop_ptr = SIMPLE_PROP(PROBE_Y, ADDR_A);

  auto leaf_asrt =
      std::make_shared<Scasrt::Asrt>("leaf_asrt", eval_prop_ptr, start_prop_ptr,
                                     stop_prop_ptr, Scasrt::ASRT_PASS_ON_HOLD);

  Scasrt::Kernel::GetInstance()->RegisterAsrt(leaf_asrt);
  CHECK(Scasrt::Kernel::GetInstance()->active_asrts_.size() == 0);
  CHECK(Scasrt::Kernel::GetInstance()->inactive_asrts_.size() == 0);
  CHECK(Scasrt::Kernel::GetInstance()->next_asrts_.size() == 1);

  Scasrt::Kernel::GetInstance()->Cleanup();
  CHECK(Scasrt::Kernel::GetInstance()->active_asrts_.size() == 1);
  CHECK(Scasrt::Kernel::GetInstance()->inactive_asrts_.size() == 0);
  CHECK(Scasrt::Kernel::GetInstance()->next_asrts_.size() == 0);
  CHECK(!Scasrt::Kernel::GetInstance()->first_cleanup_);

  tlm::tlm_generic_payload trans;
  trans.set_address(ADDR_A);
  unsigned char data[4] = {'A', 'B', 'C', 'D'};
  trans.set_data_ptr((unsigned char*)&data);

  Scasrt::Kernel::GetInstance()->SnoopNbTransportFwBgn(trans, PROBE_Y, T(0),
                                                       tlm::BEGIN_REQ);
  CHECK(leaf_asrt->spawn_count_ == 1);
  CHECK(leaf_asrt->latest_tlm_->snoop_phase_ == Scasrt::NB_TRANSPORT_FW_BGN);
  CHECK(leaf_asrt->latest_tlm_->tlm_phase_ == tlm::BEGIN_REQ);
  CHECK(Scasrt::Kernel::GetInstance()->snoop_count_ == 1);
  CHECK(Scasrt::Kernel::GetInstance()->eval_count_ == 1);
  CHECK(Scasrt::Kernel::GetInstance()->pass_count_ == 1);
  CHECK(Scasrt::Kernel::GetInstance()->fail_count_ == 0);
  CHECK(Scasrt::Kernel::GetInstance()->active_asrts_.size() == 0);
  CHECK(Scasrt::Kernel::GetInstance()->inactive_asrts_.size() == 0);
  CHECK(Scasrt::Kernel::GetInstance()->next_asrts_.size() == 0);
}

TEST_CASE("Scasrt::Kernel::SnoopNbTransportFwEnd") {
  setup_testcase();

  SPDLOG_LOGGER_INFO(spdlog::get("scasrt_test_log"),
                     "Scasrt::Kernel::SnoopNbTransportFwEnd");

  SPDLOG_LOGGER_INFO(spdlog::get("scasrt_test_log"),
                     Scasrt::Kernel::GetInstance()->ToString());

  auto start_prop_ptr = SIMPLE_PROP(PROBE_Y, ADDR_A);
  auto stop_prop_ptr = SIMPLE_PROP(PROBE_Y, ADDR_A);
  auto eval_prop_ptr = SIMPLE_PROP(PROBE_Y, ADDR_A);

  auto leaf_asrt =
      std::make_shared<Scasrt::Asrt>("leaf_asrt", eval_prop_ptr, start_prop_ptr,
                                     stop_prop_ptr, Scasrt::ASRT_PASS_ON_HOLD);

  Scasrt::Kernel::GetInstance()->RegisterAsrt(leaf_asrt);
  CHECK(Scasrt::Kernel::GetInstance()->active_asrts_.size() == 0);
  CHECK(Scasrt::Kernel::GetInstance()->inactive_asrts_.size() == 0);
  CHECK(Scasrt::Kernel::GetInstance()->next_asrts_.size() == 1);

  Scasrt::Kernel::GetInstance()->Cleanup();
  CHECK(Scasrt::Kernel::GetInstance()->active_asrts_.size() == 1);
  CHECK(Scasrt::Kernel::GetInstance()->inactive_asrts_.size() == 0);
  CHECK(Scasrt::Kernel::GetInstance()->next_asrts_.size() == 0);
  CHECK(!Scasrt::Kernel::GetInstance()->first_cleanup_);

  tlm::tlm_generic_payload trans;
  trans.set_address(ADDR_A);
  unsigned char data[4] = {'A', 'B', 'C', 'D'};
  trans.set_data_ptr((unsigned char*)&data);

  Scasrt::Kernel::GetInstance()->SnoopNbTransportFwEnd(
      trans, PROBE_Y, T(0), tlm::END_REQ, tlm::TLM_ACCEPTED);
  CHECK(leaf_asrt->spawn_count_ == 1);
  CHECK(leaf_asrt->latest_tlm_->snoop_phase_ == Scasrt::NB_TRANSPORT_FW_END);
  CHECK(leaf_asrt->latest_tlm_->tlm_phase_ == tlm::END_REQ);
  CHECK(leaf_asrt->latest_tlm_->status_ == tlm::TLM_ACCEPTED);
  CHECK(Scasrt::Kernel::GetInstance()->snoop_count_ == 1);
  CHECK(Scasrt::Kernel::GetInstance()->eval_count_ == 1);
  CHECK(Scasrt::Kernel::GetInstance()->pass_count_ == 1);
  CHECK(Scasrt::Kernel::GetInstance()->fail_count_ == 0);
  CHECK(Scasrt::Kernel::GetInstance()->active_asrts_.size() == 0);
  CHECK(Scasrt::Kernel::GetInstance()->inactive_asrts_.size() == 0);
  CHECK(Scasrt::Kernel::GetInstance()->next_asrts_.size() == 0);
}

TEST_CASE("Scasrt::Kernel::SnoopNbTransportBwBgn") {
  setup_testcase();

  SPDLOG_LOGGER_INFO(spdlog::get("scasrt_test_log"),
                     "Scasrt::Kernel::SnoopNbTransportBwBgn");

  SPDLOG_LOGGER_INFO(spdlog::get("scasrt_test_log"),
                     Scasrt::Kernel::GetInstance()->ToString());

  auto start_prop_ptr = SIMPLE_PROP(PROBE_Y, ADDR_A);
  auto stop_prop_ptr = SIMPLE_PROP(PROBE_Y, ADDR_A);
  auto eval_prop_ptr = SIMPLE_PROP(PROBE_Y, ADDR_A);

  auto leaf_asrt =
      std::make_shared<Scasrt::Asrt>("leaf_asrt", eval_prop_ptr, start_prop_ptr,
                                     stop_prop_ptr, Scasrt::ASRT_PASS_ON_HOLD);

  Scasrt::Kernel::GetInstance()->RegisterAsrt(leaf_asrt);
  CHECK(Scasrt::Kernel::GetInstance()->active_asrts_.size() == 0);
  CHECK(Scasrt::Kernel::GetInstance()->inactive_asrts_.size() == 0);
  CHECK(Scasrt::Kernel::GetInstance()->next_asrts_.size() == 1);

  Scasrt::Kernel::GetInstance()->Cleanup();
  CHECK(Scasrt::Kernel::GetInstance()->active_asrts_.size() == 1);
  CHECK(Scasrt::Kernel::GetInstance()->inactive_asrts_.size() == 0);
  CHECK(Scasrt::Kernel::GetInstance()->next_asrts_.size() == 0);
  CHECK(!Scasrt::Kernel::GetInstance()->first_cleanup_);

  tlm::tlm_generic_payload trans;
  trans.set_address(ADDR_A);
  unsigned char data[4] = {'A', 'B', 'C', 'D'};
  trans.set_data_ptr((unsigned char*)&data);

  Scasrt::Kernel::GetInstance()->SnoopNbTransportBwBgn(trans, PROBE_Y, T(0),
                                                       tlm::BEGIN_RESP);
  CHECK(leaf_asrt->spawn_count_ == 1);
  CHECK(leaf_asrt->latest_tlm_->snoop_phase_ == Scasrt::NB_TRANSPORT_BW_BGN);
  CHECK(leaf_asrt->latest_tlm_->tlm_phase_ == tlm::BEGIN_RESP);
  CHECK(Scasrt::Kernel::GetInstance()->snoop_count_ == 1);
  CHECK(Scasrt::Kernel::GetInstance()->eval_count_ == 1);
  CHECK(Scasrt::Kernel::GetInstance()->pass_count_ == 1);
  CHECK(Scasrt::Kernel::GetInstance()->fail_count_ == 0);
  CHECK(Scasrt::Kernel::GetInstance()->active_asrts_.size() == 0);
  CHECK(Scasrt::Kernel::GetInstance()->inactive_asrts_.size() == 0);
  CHECK(Scasrt::Kernel::GetInstance()->next_asrts_.size() == 0);
}

TEST_CASE("Scasrt::Kernel::SnoopNbTransportBwEnd") {
  setup_testcase();

  SPDLOG_LOGGER_INFO(spdlog::get("scasrt_test_log"),
                     "Scasrt::Kernel::SnoopNbTransportBwEnd");

  SPDLOG_LOGGER_INFO(spdlog::get("scasrt_test_log"),
                     Scasrt::Kernel::GetInstance()->ToString());

  auto start_prop_ptr = SIMPLE_PROP(PROBE_Y, ADDR_A);
  auto stop_prop_ptr = SIMPLE_PROP(PROBE_Y, ADDR_A);
  auto eval_prop_ptr = SIMPLE_PROP(PROBE_Y, ADDR_A);

  auto leaf_asrt =
      std::make_shared<Scasrt::Asrt>("leaf_asrt", eval_prop_ptr, start_prop_ptr,
                                     stop_prop_ptr, Scasrt::ASRT_PASS_ON_HOLD);

  Scasrt::Kernel::GetInstance()->RegisterAsrt(leaf_asrt);
  CHECK(Scasrt::Kernel::GetInstance()->active_asrts_.size() == 0);
  CHECK(Scasrt::Kernel::GetInstance()->inactive_asrts_.size() == 0);
  CHECK(Scasrt::Kernel::GetInstance()->next_asrts_.size() == 1);

  Scasrt::Kernel::GetInstance()->Cleanup();
  CHECK(Scasrt::Kernel::GetInstance()->active_asrts_.size() == 1);
  CHECK(Scasrt::Kernel::GetInstance()->inactive_asrts_.size() == 0);
  CHECK(Scasrt::Kernel::GetInstance()->next_asrts_.size() == 0);
  CHECK(!Scasrt::Kernel::GetInstance()->first_cleanup_);

  tlm::tlm_generic_payload trans;
  trans.set_address(ADDR_A);
  unsigned char data[4] = {'A', 'B', 'C', 'D'};
  trans.set_data_ptr((unsigned char*)&data);

  Scasrt::Kernel::GetInstance()->SnoopNbTransportBwEnd(
      trans, PROBE_Y, T(0), tlm::END_RESP, tlm::TLM_COMPLETED);
  CHECK(leaf_asrt->spawn_count_ == 1);
  CHECK(leaf_asrt->latest_tlm_->snoop_phase_ == Scasrt::NB_TRANSPORT_BW_END);
  CHECK(leaf_asrt->latest_tlm_->tlm_phase_ == tlm::END_RESP);
  CHECK(leaf_asrt->latest_tlm_->status_ == tlm::TLM_COMPLETED);
  CHECK(Scasrt::Kernel::GetInstance()->snoop_count_ == 1);
  CHECK(Scasrt::Kernel::GetInstance()->eval_count_ == 1);
  CHECK(Scasrt::Kernel::GetInstance()->pass_count_ == 1);
  CHECK(Scasrt::Kernel::GetInstance()->fail_count_ == 0);
  CHECK(Scasrt::Kernel::GetInstance()->active_asrts_.size() == 0);
  CHECK(Scasrt::Kernel::GetInstance()->inactive_asrts_.size() == 0);
  CHECK(Scasrt::Kernel::GetInstance()->next_asrts_.size() == 0);
}
