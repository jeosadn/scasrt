#include "../tst/Utils.h"

// ----------------------------------------------------------------------------
//  Unit Tests
// ----------------------------------------------------------------------------
TEST_CASE("Scasrt::Prop::Constructor") {
  setup_testcase();

  SPDLOG_LOGGER_INFO(spdlog::get("scasrt_test_log"),
                     "Scasrt::Prop::Constructor");

  auto prop_ptr = std::make_shared<Scasrt::Prop>(
      PROBE_Y, L_PROP {
        if (tlm.address_ == ADDR_A) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        };
      });

  CHECK(prop_ptr->probe_.compare(PROBE_Y) == 0);
  CHECK(prop_ptr->eval_ != nullptr);
}

TEST_CASE("Scasrt::Prop::Skip") {
  setup_testcase();

  SPDLOG_LOGGER_INFO(spdlog::get("scasrt_test_log"), "Scasrt::Prop::Pass");

  auto prop_ptr = std::make_shared<Scasrt::Prop>(
      PROBE_Y, L_PROP {
        if (tlm.address_ == ADDR_A) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        };
      });

  auto empty_history = std::vector<std::shared_ptr<const Scasrt::Tlm> >();
  CHECK(Scasrt::PROP_SKIP ==
        prop_ptr->Main(*make_tlm(ADDR_A, PROBE_X, T(0)), empty_history));
}

TEST_CASE("Scasrt::Prop::Pass") {
  setup_testcase();

  SPDLOG_LOGGER_INFO(spdlog::get("scasrt_test_log"), "Scasrt::Prop::Pass");

  auto prop_ptr = std::make_shared<Scasrt::Prop>(
      PROBE_Y, L_PROP {
        if (tlm.address_ == ADDR_A) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        };
      });

  auto empty_history = std::vector<std::shared_ptr<const Scasrt::Tlm> >();
  CHECK(Scasrt::PROP_TRUE ==
        prop_ptr->Main(*make_tlm(ADDR_A, PROBE_Y, T(0)), empty_history));
}

TEST_CASE("Scasrt::Prop::Fail") {
  setup_testcase();

  SPDLOG_LOGGER_INFO(spdlog::get("scasrt_test_log"), "Scasrt::Prop::Fail");

  auto prop_ptr = std::make_shared<Scasrt::Prop>(
      PROBE_Y, L_PROP {
        if (tlm.address_ == ADDR_A) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        };
      });

  auto empty_history = std::vector<std::shared_ptr<const Scasrt::Tlm> >();
  CHECK(Scasrt::PROP_FALSE ==
        prop_ptr->Main(*make_tlm(ADDR_B, PROBE_Y, T(0)), empty_history));
}

TEST_CASE("Scasrt::Prop::History") {
  setup_testcase();

  SPDLOG_LOGGER_INFO(spdlog::get("scasrt_test_log"), "Scasrt::Prop::History");

  auto prop_ptr = std::make_shared<Scasrt::Prop>(
      PROBE_Y, L_PROP {
        if (tlm.address_ == history.back()->address_) {
          return Scasrt::PROP_TRUE;
        } else {
          return Scasrt::PROP_FALSE;
        };
      });

  auto history = std::vector<std::shared_ptr<const Scasrt::Tlm> >();
  history.push_back(make_tlm(ADDR_A, PROBE_Y, T(0)));
  CHECK(Scasrt::PROP_TRUE ==
        prop_ptr->Main(*make_tlm(ADDR_A, PROBE_Y, T(1)), history));
}
