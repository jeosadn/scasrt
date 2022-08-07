#include "Tlm.h"

namespace Scasrt {
Tlm::Tlm(const tlm::tlm_generic_payload& trans,
         const std::string& observation_point,
         const sc_core::sc_time& time_stamp, const SnoopPhase& snoop_phase,
         const tlm::tlm_phase& tlm_phase, const tlm::tlm_sync_enum& status)
    : observation_point_(observation_point),
      time_stamp_(time_stamp),
      address_(trans.get_address()),
      command_(trans.get_command()),
      data_(trans.get_data_ptr()),
      data_i_(*(reinterpret_cast<unsigned int*>(trans.get_data_ptr()))),
      length_(trans.get_data_length()),
      response_status_(trans.get_response_status()),
      dmi_(trans.is_dmi_allowed()),
      byte_enable_(trans.get_byte_enable_ptr()),
      byte_enable_length_(trans.get_byte_enable_length()),
      streaming_width_(trans.get_streaming_width()),
      gp_option_(trans.get_gp_option()),
      tlm_phase_(tlm_phase),
      snoop_phase_(snoop_phase),
      status_(status) {
  SPDLOG_LOGGER_DEBUG(spdlog::get("scasrt_log"),
                      "Constructed Tlm for addr:{}, command:{}", address_,
                      command_);
}
}  // namespace Scasrt
