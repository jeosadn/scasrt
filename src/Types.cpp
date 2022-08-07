#include "Types.h"

std::string sync_to_string(tlm::tlm_sync_enum& status) {
  switch (status) {
    case tlm::TLM_ACCEPTED:
      return "TLM_OK_RESPONSE";
    case tlm::TLM_UPDATED:
      return "TLM_INCOMPLETE_RESPONSE";
    case tlm::TLM_COMPLETED:
      return "TLM_GENERIC_ERROR_RESPONSE";
  }
  return "TLM_UNKNOWN_RESPONSE";
}
