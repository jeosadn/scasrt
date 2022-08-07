#include "../tst/Utils.h"

// ----------------------------------------------------------------------------
//  Unit tests
// ----------------------------------------------------------------------------
TEST_CASE("Scasrt::Asrt::Leaf constructor") {
  setup_testcase();

  SPDLOG_LOGGER_INFO(spdlog::get("scasrt_test_log"),
                     "Scasrt::Asrt::Leaf constructor");

  auto start_prop_ptr = TIME_PROP(PROBE_Y, T(0));
  auto stop_prop_ptr = TIME_PROP(PROBE_Y, T(1));
  auto eval_prop_ptr = SIMPLE_PROP(PROBE_Y, ADDR_A);

  auto leaf_asrt =
      std::make_shared<Scasrt::Asrt>("leaf_asrt", eval_prop_ptr, start_prop_ptr,
                                     stop_prop_ptr, Scasrt::ASRT_PASS_ON_HOLD);

  CHECK(leaf_asrt->id_ == 0);
  CHECK(!leaf_asrt->id_set_);
  CHECK(leaf_asrt->start_prop_ptr_ != nullptr);
  CHECK(leaf_asrt->stop_prop_ptr_ != nullptr);
  CHECK(leaf_asrt->eval_prop_ptr_ != nullptr);
  CHECK(leaf_asrt->state_ == Scasrt::ASRT_DISABLED);
  CHECK(!leaf_asrt->spawn_next_asrt_);
  CHECK(leaf_asrt->spawn_count_ == 0);
  CHECK(leaf_asrt->is_leaf_);
  CHECK(leaf_asrt->next_asrt_ == nullptr);
  CHECK(leaf_asrt->history_.size() == 0);
  CHECK(leaf_asrt->latest_tlm_ == nullptr);
  CHECK(leaf_asrt->criteria_ == Scasrt::ASRT_PASS_ON_HOLD);
}

TEST_CASE("Scasrt::Asrt::Non-leaf constructor") {
  setup_testcase();

  SPDLOG_LOGGER_INFO(spdlog::get("scasrt_test_log"),
                     "Scasrt::Asrt::Non-leaf constructor");

  auto start_prop_ptr = TIME_PROP(PROBE_Y, T(0));
  auto stop_prop_ptr = TIME_PROP(PROBE_Y, T(1));
  auto eval_prop_ptr = SIMPLE_PROP(PROBE_Y, ADDR_A);

  auto leaf_asrt =
      std::make_shared<Scasrt::Asrt>("leaf_asrt", eval_prop_ptr, start_prop_ptr,
                                     stop_prop_ptr, Scasrt::ASRT_PASS_ON_HOLD);
  auto non_leaf_asrt = std::make_shared<Scasrt::Asrt>(
      "non_leaf_asrt", eval_prop_ptr, start_prop_ptr, stop_prop_ptr, leaf_asrt);

  CHECK(leaf_asrt->id_ == 0);
  CHECK(!leaf_asrt->id_set_);
  CHECK(leaf_asrt->start_prop_ptr_ != nullptr);
  CHECK(leaf_asrt->stop_prop_ptr_ != nullptr);
  CHECK(leaf_asrt->eval_prop_ptr_ != nullptr);
  CHECK(leaf_asrt->state_ == Scasrt::ASRT_DISABLED);
  CHECK(!leaf_asrt->spawn_next_asrt_);
  CHECK(leaf_asrt->spawn_count_ == 0);
  CHECK(leaf_asrt->is_leaf_);
  CHECK(leaf_asrt->next_asrt_ == nullptr);
  CHECK(leaf_asrt->history_.size() == 0);
  CHECK(leaf_asrt->latest_tlm_ == nullptr);
  CHECK(leaf_asrt->criteria_ == Scasrt::ASRT_PASS_ON_HOLD);

  CHECK(non_leaf_asrt->id_ == 0);
  CHECK(!non_leaf_asrt->id_set_);
  CHECK(non_leaf_asrt->start_prop_ptr_ != nullptr);
  CHECK(non_leaf_asrt->stop_prop_ptr_ != nullptr);
  CHECK(non_leaf_asrt->eval_prop_ptr_ != nullptr);
  CHECK(non_leaf_asrt->state_ == Scasrt::ASRT_DISABLED);
  CHECK(!non_leaf_asrt->spawn_next_asrt_);
  CHECK(non_leaf_asrt->spawn_count_ == 0);
  CHECK(!non_leaf_asrt->is_leaf_);
  CHECK(non_leaf_asrt->next_asrt_ != nullptr);
  CHECK(non_leaf_asrt->history_.size() == 0);
  CHECK(non_leaf_asrt->latest_tlm_ == nullptr);
  CHECK(non_leaf_asrt->criteria_ == Scasrt::ASRT_PASS_INVALID);
}

TEST_CASE("Scasrt::Asrt::SetId") {
  setup_testcase();

  SPDLOG_LOGGER_INFO(spdlog::get("scasrt_test_log"), "Scasrt::Asrt::SetId");

  auto start_prop_ptr = TIME_PROP(PROBE_Y, T(0));
  auto stop_prop_ptr = TIME_PROP(PROBE_Y, T(1));
  auto eval_prop_ptr = SIMPLE_PROP(PROBE_Y, ADDR_A);

  auto leaf_asrt =
      std::make_shared<Scasrt::Asrt>("leaf_asrt", eval_prop_ptr, start_prop_ptr,
                                     stop_prop_ptr, Scasrt::ASRT_PASS_ON_HOLD);

  CHECK(leaf_asrt->id_ == 0);
  CHECK(!leaf_asrt->id_set_);
  leaf_asrt->SetId(4);
  CHECK(leaf_asrt->id_ == 4);
  CHECK(leaf_asrt->id_set_);
}

TEST_CASE("Scasrt::Asrt::Name") {
  setup_testcase();

  SPDLOG_LOGGER_INFO(spdlog::get("scasrt_test_log"), "Scasrt::Asrt::Name");

  auto start_prop_ptr = TIME_PROP(PROBE_Y, T(0));
  auto stop_prop_ptr = TIME_PROP(PROBE_Y, T(1));
  auto eval_prop_ptr = SIMPLE_PROP(PROBE_Y, ADDR_A);

  auto leaf_asrt =
      std::make_shared<Scasrt::Asrt>("leaf_asrt", eval_prop_ptr, start_prop_ptr,
                                     stop_prop_ptr, Scasrt::ASRT_PASS_ON_HOLD);

  CHECK(leaf_asrt->Name().compare("leaf_asrt_#") == 0);
  leaf_asrt->SetId(4);
  CHECK(leaf_asrt->Name().compare("leaf_asrt_4") == 0);
}

TEST_CASE("Scasrt::Asrt::GenNextAsrt") {
  setup_testcase();

  SPDLOG_LOGGER_INFO(spdlog::get("scasrt_test_log"),
                     "Scasrt::Asrt::GenNextAsrt");

  auto start_prop_ptr = TIME_PROP(PROBE_Y, T(0));
  auto stop_prop_ptr = TIME_PROP(PROBE_Y, T(1));
  auto eval_prop_ptr = SIMPLE_PROP(PROBE_Y, ADDR_A);

  auto leaf_asrt =
      std::make_shared<Scasrt::Asrt>("leaf_asrt", eval_prop_ptr, start_prop_ptr,
                                     stop_prop_ptr, Scasrt::ASRT_PASS_ON_HOLD);
  auto non_leaf_asrt = std::make_shared<Scasrt::Asrt>(
      "non_leaf_asrt", eval_prop_ptr, start_prop_ptr, stop_prop_ptr, leaf_asrt);

  auto next_non_leaf_asrt = non_leaf_asrt->GenNextAsrt();
  CHECK(non_leaf_asrt->spawn_count_ == 1);
  CHECK(next_non_leaf_asrt->id_ == 0);
  CHECK(!next_non_leaf_asrt->id_set_);
  CHECK(next_non_leaf_asrt->start_prop_ptr_ != nullptr);
  CHECK(next_non_leaf_asrt->stop_prop_ptr_ != nullptr);
  CHECK(next_non_leaf_asrt->eval_prop_ptr_ != nullptr);
  CHECK(next_non_leaf_asrt->state_ == Scasrt::ASRT_DISABLED);
  CHECK(!next_non_leaf_asrt->spawn_next_asrt_);
  CHECK(next_non_leaf_asrt->spawn_count_ == 0);
  CHECK(next_non_leaf_asrt->is_leaf_);
  CHECK(next_non_leaf_asrt->next_asrt_ == nullptr);
  CHECK(next_non_leaf_asrt->history_.size() == 1);
  CHECK(next_non_leaf_asrt->latest_tlm_ == nullptr);
  CHECK(next_non_leaf_asrt->criteria_ == Scasrt::ASRT_PASS_ON_HOLD);

  auto next_leaf_asrt = leaf_asrt->GenNextAsrt();
  CHECK(leaf_asrt->spawn_count_ == 1);
  CHECK(next_leaf_asrt->id_ == 0);
  CHECK(!next_leaf_asrt->id_set_);
  CHECK(next_leaf_asrt->start_prop_ptr_ != nullptr);
  CHECK(next_leaf_asrt->stop_prop_ptr_ != nullptr);
  CHECK(next_leaf_asrt->eval_prop_ptr_ != nullptr);
  CHECK(next_leaf_asrt->state_ == Scasrt::ASRT_MATCHED);
  CHECK(!next_leaf_asrt->spawn_next_asrt_);
  CHECK(next_leaf_asrt->spawn_count_ == 0);
  CHECK(next_leaf_asrt->is_leaf_);
  CHECK(next_leaf_asrt->next_asrt_ == nullptr);
  CHECK(next_leaf_asrt->history_.size() == 1);
  CHECK(next_leaf_asrt->latest_tlm_ == nullptr);
  CHECK(next_leaf_asrt->criteria_ == Scasrt::ASRT_PASS_ON_HOLD);
}

TEST_CASE("Scasrt::Asrt::Main") {
  setup_testcase();

  SPDLOG_LOGGER_INFO(spdlog::get("scasrt_test_log"), "Scasrt::Asrt::Main");

  SPDLOG_LOGGER_INFO(spdlog::get("scasrt_test_log"),
                     "Asrt starts on 1 evals on 2 stops on 3");

  auto start_prop_ptr = SIMPLE_PROP(PROBE_Y, ADDR_A);
  auto eval_prop_ptr = SIMPLE_PROP(PROBE_Y, ADDR_B);
  auto stop_prop_ptr = SIMPLE_PROP(PROBE_Y, ADDR_C);

  auto asrt =
      std::make_shared<Scasrt::Asrt>("asrt", eval_prop_ptr, start_prop_ptr,
                                     stop_prop_ptr, Scasrt::ASRT_PASS_ON_HOLD);
  CHECK(Scasrt::ASRT_DISABLED == asrt->state_);

  asrt->Main(make_tlm(ADDR_A, PROBE_Y, T(0)));
  CHECK(Scasrt::ASRT_ENABLED == asrt->state_);
  CHECK(!asrt->spawn_next_asrt_);
  CHECK(asrt->latest_tlm_ == nullptr);

  asrt->Main(make_tlm(ADDR_B, PROBE_Y, T(0)));
  CHECK(asrt->spawn_next_asrt_);
  CHECK(asrt->latest_tlm_ != nullptr);
  CHECK(Scasrt::ASRT_ENABLED == asrt->state_);

  asrt->Main(make_tlm(ADDR_C, PROBE_Y, T(0)));
  CHECK(asrt->spawn_next_asrt_);
  CHECK(asrt->latest_tlm_ != nullptr);
  CHECK(Scasrt::ASRT_EXPIRED == asrt->state_);
}

TEST_CASE("Scasrt::Asrt::GetStatus") {
  setup_testcase();

  SPDLOG_LOGGER_INFO(spdlog::get("scasrt_test_log"), "Scasrt::Asrt::GetStatus");

  auto start_prop_ptr = TIME_PROP(PROBE_Y, T(0));
  auto stop_prop_ptr = TIME_PROP(PROBE_Y, T(1));
  auto eval_prop_ptr = SIMPLE_PROP(PROBE_Y, ADDR_A);

  auto leaf_asrt =
      std::make_shared<Scasrt::Asrt>("leaf_asrt", eval_prop_ptr, start_prop_ptr,
                                     stop_prop_ptr, Scasrt::ASRT_PASS_ON_HOLD);
  CHECK(leaf_asrt->GetStatus() == Scasrt::ASRT_HOLDS_SOFTLY);

  leaf_asrt->state_ = Scasrt::ASRT_ENABLED;
  CHECK(leaf_asrt->GetStatus() == Scasrt::ASRT_HOLDS_SOFTLY);

  leaf_asrt->state_ = Scasrt::ASRT_EXPIRED;
  CHECK(leaf_asrt->GetStatus() == Scasrt::ASRT_DOESNT_HOLD);

  leaf_asrt->state_ = Scasrt::ASRT_MATCHED;
  CHECK(leaf_asrt->GetStatus() == Scasrt::ASRT_HOLDS_STRONGLY);

  leaf_asrt->spawn_count_ = 1;
  CHECK(leaf_asrt->GetStatus() == Scasrt::ASRT_NO_STATUS);
}

TEST_CASE("Scasrt::Asrt::GetCriteria") {
  setup_testcase();

  SPDLOG_LOGGER_INFO(spdlog::get("scasrt_test_log"),
                     "Scasrt::Asrt::GetCriteria");

  auto start_prop_ptr = TIME_PROP(PROBE_Y, T(0));
  auto stop_prop_ptr = TIME_PROP(PROBE_Y, T(1));
  auto eval_prop_ptr = SIMPLE_PROP(PROBE_Y, ADDR_A);

  auto leaf_asrt =
      std::make_shared<Scasrt::Asrt>("leaf_asrt", eval_prop_ptr, start_prop_ptr,
                                     stop_prop_ptr, Scasrt::ASRT_PASS_ON_HOLD);
  auto non_leaf_asrt = std::make_shared<Scasrt::Asrt>(
      "non_leaf_asrt", eval_prop_ptr, start_prop_ptr, stop_prop_ptr, leaf_asrt);

  CHECK(leaf_asrt->GetCriteria() == Scasrt::ASRT_PASS_ON_HOLD);
  CHECK(non_leaf_asrt->GetCriteria() == Scasrt::ASRT_PASS_ON_HOLD);

  leaf_asrt->criteria_ = Scasrt::ASRT_PASS_ON_HOLD_STRONG;
  CHECK(leaf_asrt->GetCriteria() == Scasrt::ASRT_PASS_ON_HOLD_STRONG);
  CHECK(non_leaf_asrt->GetCriteria() == Scasrt::ASRT_PASS_ON_HOLD_STRONG);

  leaf_asrt->criteria_ = Scasrt::ASRT_PASS_ON_NOTHOLD;
  CHECK(leaf_asrt->GetCriteria() == Scasrt::ASRT_PASS_ON_NOTHOLD);
  CHECK(non_leaf_asrt->GetCriteria() == Scasrt::ASRT_PASS_ON_NOTHOLD);

  leaf_asrt->criteria_ = Scasrt::ASRT_PASS_ON_NOTHOLD_STRONG;
  CHECK(leaf_asrt->GetCriteria() == Scasrt::ASRT_PASS_ON_NOTHOLD_STRONG);
  CHECK(non_leaf_asrt->GetCriteria() == Scasrt::ASRT_PASS_ON_NOTHOLD_STRONG);
}

TEST_CASE("Scasrt::Asrt::Passes") {
  setup_testcase();

  SPDLOG_LOGGER_INFO(spdlog::get("scasrt_test_log"), "Scasrt::Asrt::Passes");

  auto start_prop_ptr = TIME_PROP(PROBE_Y, T(0));
  auto stop_prop_ptr = TIME_PROP(PROBE_Y, T(1));
  auto eval_prop_ptr = SIMPLE_PROP(PROBE_Y, ADDR_A);

  auto leaf_asrt =
      std::make_shared<Scasrt::Asrt>("leaf_asrt", eval_prop_ptr, start_prop_ptr,
                                     stop_prop_ptr, Scasrt::ASRT_PASS_ON_HOLD);

  CHECK(leaf_asrt->Passes(Scasrt::ASRT_HOLDS_STRONGLY,
                          Scasrt::ASRT_PASS_ON_HOLD));
  CHECK(!leaf_asrt->Passes(Scasrt::ASRT_HOLDS_STRONGLY,
                           Scasrt::ASRT_PASS_ON_NOTHOLD));
  CHECK(leaf_asrt->Passes(Scasrt::ASRT_HOLDS_STRONGLY,
                          Scasrt::ASRT_PASS_ON_HOLD_STRONG));
  CHECK(!leaf_asrt->Passes(Scasrt::ASRT_HOLDS_STRONGLY,
                           Scasrt::ASRT_PASS_ON_NOTHOLD_STRONG));

  CHECK(
      leaf_asrt->Passes(Scasrt::ASRT_HOLDS_SOFTLY, Scasrt::ASRT_PASS_ON_HOLD));
  CHECK(leaf_asrt->Passes(Scasrt::ASRT_HOLDS_SOFTLY,
                          Scasrt::ASRT_PASS_ON_NOTHOLD));
  CHECK(!leaf_asrt->Passes(Scasrt::ASRT_HOLDS_SOFTLY,
                           Scasrt::ASRT_PASS_ON_HOLD_STRONG));
  CHECK(!leaf_asrt->Passes(Scasrt::ASRT_HOLDS_SOFTLY,
                           Scasrt::ASRT_PASS_ON_NOTHOLD_STRONG));

  CHECK(
      !leaf_asrt->Passes(Scasrt::ASRT_DOESNT_HOLD, Scasrt::ASRT_PASS_ON_HOLD));
  CHECK(leaf_asrt->Passes(Scasrt::ASRT_DOESNT_HOLD,
                          Scasrt::ASRT_PASS_ON_NOTHOLD));
  CHECK(!leaf_asrt->Passes(Scasrt::ASRT_DOESNT_HOLD,
                           Scasrt::ASRT_PASS_ON_HOLD_STRONG));
  CHECK(leaf_asrt->Passes(Scasrt::ASRT_DOESNT_HOLD,
                          Scasrt::ASRT_PASS_ON_NOTHOLD_STRONG));
}

TEST_CASE("Scasrt::Asrt::ToString") {
  setup_testcase();

  SPDLOG_LOGGER_INFO(spdlog::get("scasrt_test_log"), "Scasrt::Asrt::ToString");

  auto start_prop_ptr = TIME_PROP(PROBE_Y, T(0));
  auto stop_prop_ptr = TIME_PROP(PROBE_Y, T(1));
  auto eval_prop_ptr = SIMPLE_PROP(PROBE_Y, ADDR_A);

  auto leaf_asrt =
      std::make_shared<Scasrt::Asrt>("leaf_asrt", eval_prop_ptr, start_prop_ptr,
                                     stop_prop_ptr, Scasrt::ASRT_PASS_ON_HOLD);

  SPDLOG_LOGGER_INFO(spdlog::get("scasrt_test_log"), "{}",
                     leaf_asrt->ToString());
  CHECK(leaf_asrt->ToString().compare(
            "leaf_asrt_#, is_leaf_=true, spawn_count_=0") == 0);
}
