#include "Asrt.h"

namespace Scasrt {
Asrt::Asrt(const std::string &name, std::shared_ptr<Prop> eval_prop_ptr,
           std::shared_ptr<Prop> start_prop_ptr,
           std::shared_ptr<Prop> stop_prop_ptr, AsrtPass criteria)
    : name_(name),
      id_(0),
      id_set_(false),
      start_prop_ptr_(start_prop_ptr),
      stop_prop_ptr_(stop_prop_ptr),
      eval_prop_ptr_(eval_prop_ptr),
      state_(ASRT_DISABLED),
      spawn_next_asrt_(false),
      spawn_count_(0),
      is_leaf_(true),
      next_asrt_(nullptr),
      history_(),
      latest_tlm_(),
      criteria_(criteria) {
  SPDLOG_LOGGER_DEBUG(spdlog::get("scasrt_log"), "Constructed {}", Name());
}

Asrt::Asrt(const std::string &name, std::shared_ptr<Prop> eval_prop_ptr,
           std::shared_ptr<Prop> start_prop_ptr,
           std::shared_ptr<Prop> stop_prop_ptr, std::shared_ptr<Asrt> next_asrt)
    : name_(name),
      id_(0),
      id_set_(false),
      start_prop_ptr_(start_prop_ptr),
      stop_prop_ptr_(stop_prop_ptr),
      eval_prop_ptr_(eval_prop_ptr),
      state_(ASRT_DISABLED),
      spawn_next_asrt_(false),
      spawn_count_(0),
      is_leaf_(false),
      next_asrt_(next_asrt),
      history_(),
      latest_tlm_(),
      criteria_(ASRT_PASS_INVALID) {
  SPDLOG_LOGGER_DEBUG(spdlog::get("scasrt_log"), "Constructed {}", name_);
}

Asrt::Asrt(const Asrt &asrt)
    : name_(asrt.name_),
      id_(asrt.id_),
      id_set_(asrt.id_set_),
      start_prop_ptr_(asrt.start_prop_ptr_),
      stop_prop_ptr_(asrt.stop_prop_ptr_),
      eval_prop_ptr_(asrt.eval_prop_ptr_),
      state_(asrt.state_),
      spawn_next_asrt_(asrt.spawn_next_asrt_),
      spawn_count_(asrt.spawn_count_),
      is_leaf_(asrt.is_leaf_),
      next_asrt_(asrt.next_asrt_),
      history_(asrt.history_),
      latest_tlm_(asrt.latest_tlm_),
      criteria_(asrt.criteria_) {}

std::string Asrt::Name() {
  if (id_set_) {
    return fmt::format("{}_{}", name_, id_);
  } else {
    return fmt::format("{}_#", name_);
  }
}

std::string Asrt::ToString() {
  std::string result = fmt::format("{}, is_leaf_={}, spawn_count_={}", Name(),
                                   is_leaf_, spawn_count_);
  return result;
}

void Asrt::SetId(int id) {
  if (id_set_) {
    SPDLOG_LOGGER_ERROR(spdlog::get("scasrt_log"),
                        "ID is already set on asrt: {}", ToString());
    exit(1);
  }

  id_set_ = true;
  id_ = id;
}

std::shared_ptr<Asrt> Asrt::GenNextAsrt() {
  SPDLOG_LOGGER_DEBUG(spdlog::get("scasrt_log"), "Spawning an assert from {}",
                      Name());
  spawn_next_asrt_ = false;
  spawn_count_ += 1;
  std::shared_ptr<Asrt> result;

  if (is_leaf_) {
    result = std::shared_ptr<Asrt>(new Scasrt::Asrt(*this));
    result->id_set_ = false;
    result->state_ = ASRT_MATCHED;
    result->spawn_count_ = 0;
  } else {
    result = std::shared_ptr<Asrt>(new Scasrt::Asrt(*next_asrt_.get()));
    result->history_.insert(result->history_.end(), history_.begin(),
                            history_.end());
  }
  result->history_.push_back(latest_tlm_);

  SPDLOG_LOGGER_DEBUG(spdlog::get("scasrt_log"), "{} spawning an {} asrt",
                      Name(), result->Name());
  SPDLOG_LOGGER_DEBUG(spdlog::get("scasrt_log"),
                      "Generated {} history size: {}", result->Name(),
                      result->history_.size());
  return result;
}

AsrtPass Asrt::GetCriteria() {
  if (criteria_ != ASRT_PASS_INVALID) {
    return criteria_;
  }

  if (!is_leaf_) {
    return next_asrt_->GetCriteria();
  }

  SPDLOG_LOGGER_ERROR(
      spdlog::get("scasrt_log"),
      "Invalid combination of criteria({}) and is_leaf({}) in {}",
      asrt_pass_to_name[criteria_], is_leaf_, Name());

  exit(1);
}

AsrtStatus Asrt::GetStatus() {
  auto status = AsrtStatus();

  if (spawn_count_ != 0) {
    status = ASRT_NO_STATUS;
  } else if (state_ == ASRT_DISABLED || state_ == ASRT_ENABLED) {
    status = ASRT_HOLDS_SOFTLY;
  } else if (state_ == ASRT_EXPIRED) {
    status = ASRT_DOESNT_HOLD;
  } else if (state_ == ASRT_MATCHED) {
    status = ASRT_HOLDS_STRONGLY;
  } else {
    SPDLOG_LOGGER_ERROR(spdlog::get("scasrt_log"), "Unexpected state %s in %s",
                        asrt_state_to_name[state_], Name());
    exit(1);
  }

  return status;
}

bool Asrt::Passes(AsrtStatus status, AsrtPass criteria) {
  if (status != ASRT_HOLDS_STRONGLY && status != ASRT_HOLDS_SOFTLY &&
      status != ASRT_DOESNT_HOLD) {
    SPDLOG_LOGGER_ERROR(spdlog::get("scasrt_log"), "Unexpected status %s in %s",
                        asrt_status_to_name[status], Name());
    exit(1);
  }

  if (criteria == ASRT_PASS_ON_HOLD) {
    if (status == ASRT_HOLDS_SOFTLY or status == ASRT_HOLDS_STRONGLY) {
      return true;
    } else {
      return false;
    }
  } else if (criteria == ASRT_PASS_ON_NOTHOLD) {
    if (status == ASRT_HOLDS_SOFTLY or status == ASRT_DOESNT_HOLD) {
      return true;
    } else {
      return false;
    }
  } else if (criteria == ASRT_PASS_ON_HOLD_STRONG) {
    if (status == ASRT_HOLDS_STRONGLY) {
      return true;
    } else {
      return false;
    }
  } else if (criteria == ASRT_PASS_ON_NOTHOLD_STRONG) {
    if (status == ASRT_DOESNT_HOLD) {
      return true;
    } else {
      return false;
    }
  } else {
    SPDLOG_LOGGER_ERROR(spdlog::get("scasrt_log"),
                        "Unexpected criteria %s in %s when processing Eot",
                        Name(), state_);
    exit(1);
  }
}

void Asrt::Main(std::shared_ptr<const Tlm> tlm_ptr) {
  SPDLOG_LOGGER_DEBUG(spdlog::get("scasrt_log"),
                      "Executing {}, entry state = {}", Name(),
                      asrt_state_to_name[state_]);

  if (state_ == ASRT_EXPIRED) {
    SPDLOG_LOGGER_ERROR(spdlog::get("scasrt_log"), "Asrt already expired: {}",
                        ToString());
    return;
  }

  if (state_ == ASRT_DISABLED) {
    if (PROP_TRUE == start_prop_ptr_->Main(*tlm_ptr, history_)) {
      state_ = ASRT_ENABLED;
      SPDLOG_LOGGER_DEBUG(spdlog::get("scasrt_log"), "{} moving to state = {}",
                          Name(), asrt_state_to_name[state_]);
    } else {
      return;
    }
  }

  if (state_ == ASRT_ENABLED) {
    if (PROP_TRUE == eval_prop_ptr_->Main(*tlm_ptr, history_)) {
      latest_tlm_ = tlm_ptr;
      spawn_next_asrt_ = true;
      SPDLOG_LOGGER_DEBUG(spdlog::get("scasrt_log"), "{} eval is true", Name());
    }
  }

  if (PROP_TRUE == stop_prop_ptr_->Main(*tlm_ptr, history_)) {
    state_ = ASRT_EXPIRED;
    SPDLOG_LOGGER_DEBUG(spdlog::get("scasrt_log"), "{} moving to state = {}",
                        Name(), asrt_state_to_name[state_]);
    return;
  }
}
}  // namespace Scasrt
