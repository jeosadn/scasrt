#ifndef TST_UTILS
#define TST_UTILS
#include <memory>
#include <string>
#include <vector>

#include "Kernel.h"
#include "Tlm.h"
#include "Types.h"
#include "spdlog/spdlog.h"
#include "tlm_core/tlm_2/tlm_generic_payload/tlm_gp.h"

// ----------------------------------------------------------------------------
//  Utilities for unit tests
// ----------------------------------------------------------------------------
#define T(time) sc_core::sc_time(time, sc_core::SC_PS)
#define ADDR_A 0x00000001
#define ADDR_B 0x00000002
#define ADDR_C 0x00000003
#define PROBE_Y "top"
#define PROBE_X "bot"

#define SIMPLE_PROP(probe, addr)                                             \
  std::make_shared<Scasrt::Prop>(                                            \
      probe, [](const Scasrt::Tlm& tlm,                                      \
                std::vector<std::shared_ptr<const Scasrt::Tlm> >& history) { \
        if (tlm.address_ == addr) {                                          \
          return Scasrt::PROP_TRUE;                                          \
        } else {                                                             \
          return Scasrt::PROP_FALSE;                                         \
        }                                                                    \
      })

#define TIME_PROP(probe, time)                                               \
  std::make_shared<Scasrt::Prop>(                                            \
      probe, [](const Scasrt::Tlm& tlm,                                      \
                std::vector<std::shared_ptr<const Scasrt::Tlm> >& history) { \
        if (tlm.time_stamp_ > history.back()->time_stamp_ + time) {          \
          return Scasrt::PROP_TRUE;                                          \
        } else {                                                             \
          return Scasrt::PROP_FALSE;                                         \
        }                                                                    \
      })

std::shared_ptr<const Scasrt::Tlm> make_tlm(const sc_dt::uint64& address,
                                            const std::string& probe,
                                            const sc_core::sc_time& time);

void setup_testcase();

#endif
