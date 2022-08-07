#pragma once

#ifndef SCA_TLM_H
#define SCA_TLM_H

#include <string>

#include "Types.h"
#include "sysc/kernel/sc_time.h"

namespace Scasrt {
class Tlm {
 public:
  const std::string observation_point_;
  const sc_core::sc_time time_stamp_;

  // Default TLM fields
  const sc_dt::uint64 address_;
  const tlm::tlm_command command_;
  const unsigned char* data_;
  const unsigned int data_i_;
  const unsigned int length_;
  const tlm::tlm_response_status response_status_;
  const bool dmi_;
  const unsigned char* byte_enable_;
  const unsigned int byte_enable_length_;
  const unsigned int streaming_width_;
  const tlm::tlm_gp_option gp_option_;

  // SCA additional fields
  const tlm::tlm_phase tlm_phase_;
  const SnoopPhase snoop_phase_;
  const tlm::tlm_sync_enum status_;

  explicit Tlm() = delete;
  explicit Tlm(const tlm::tlm_generic_payload& trans,
               const std::string& observation_point = "",
               const sc_core::sc_time& time = sc_core::sc_time(0,
                                                               sc_core::SC_PS),
               const SnoopPhase& snoop_phase = SNOOP_PHASE_INVALID,
               const tlm::tlm_phase& phase = tlm::UNINITIALIZED_PHASE,
               const tlm::tlm_sync_enum& status = tlm::TLM_ACCEPTED);
};
}  // namespace Scasrt

#endif
