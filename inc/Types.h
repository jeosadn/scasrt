#pragma once

#ifndef SCA_TYPES_H
#define SCA_TYPES_H

#include <string>
#include <tlm>

namespace Scasrt {
class Tlm;
class Asrt;
class Kernel;

#define MAKE_ENUM(VAR) VAR,
#define MAKE_STRINGS(VAR) #VAR,

#define SNOOP_PHASE(DO)   \
  DO(SNOOP_PHASE_INVALID) \
  DO(B_TRANSPORT_BGN)     \
  DO(B_TRANSPORT_END)     \
  DO(NB_TRANSPORT_FW_BGN) \
  DO(NB_TRANSPORT_FW_END) \
  DO(NB_TRANSPORT_BW_BGN) \
  DO(NB_TRANSPORT_BW_END)

enum SnoopPhase { SNOOP_PHASE(MAKE_ENUM) };

const std::string snoop_phase_to_name[] = {SNOOP_PHASE(MAKE_STRINGS)};

#define ASRT_STATUS(DO)   \
  DO(ASRT_STATUS_INVALID) \
  DO(ASRT_HOLDS_STRONGLY) \
  DO(ASRT_HOLDS_SOFTLY)   \
  DO(ASRT_DOESNT_HOLD)    \
  DO(ASRT_NO_STATUS)

enum AsrtStatus { ASRT_STATUS(MAKE_ENUM) };

const std::string asrt_status_to_name[] = {ASRT_STATUS(MAKE_STRINGS)};

#define PROP_RESULT(DO)   \
  DO(PROP_RESULT_INVALID) \
  DO(PROP_TRUE)           \
  DO(PROP_FALSE)          \
  DO(PROP_SKIP)

enum PropResult { PROP_RESULT(MAKE_ENUM) };

const std::string prop_result_to_name[] = {PROP_RESULT(MAKE_STRINGS)};

#define ASRT_STATE(DO)   \
  DO(ASRT_STATE_INVALID) \
  DO(ASRT_DISABLED)      \
  DO(ASRT_ENABLED)       \
  DO(ASRT_EXPIRED)       \
  DO(ASRT_MATCHED)

enum AsrtState { ASRT_STATE(MAKE_ENUM) };

const std::string asrt_state_to_name[] = {ASRT_STATE(MAKE_STRINGS)};

#define ASRT_PASS(DO)          \
  DO(ASRT_PASS_INVALID)        \
  DO(ASRT_PASS_ON_HOLD)        \
  DO(ASRT_PASS_ON_HOLD_STRONG) \
  DO(ASRT_PASS_ON_NOTHOLD)     \
  DO(ASRT_PASS_ON_NOTHOLD_STRONG)

enum AsrtPass { ASRT_PASS(MAKE_ENUM) };

const std::string asrt_pass_to_name[] = {ASRT_PASS(MAKE_STRINGS)};

std::string sync_to_string(tlm::tlm_sync_enum &status);
}  // namespace Scasrt

#endif
