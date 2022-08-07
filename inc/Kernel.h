#pragma once

#ifndef SCA_KERNEL_H
#define SCA_KERNEL_H

#include <memory>
#include <string>
#include <tlm>
#include <vector>

#include "Asrt.h"
#include "sysc/kernel/sc_time.h"

namespace Scasrt {
class Kernel {
 public:  // private:
  static Kernel* pinstance_;
  static int next_id_;
  static int snoop_count_;
  static int eval_count_;
  static int pass_count_;
  static int fail_count_;
  static bool first_cleanup_;
  static bool print_asrts_;

 protected:
  Kernel() {}

  ~Kernel() {}

 public:
  Kernel(Kernel& other) = delete;
  void operator=(const Kernel&) = delete;

  static Kernel* GetInstance();

  static std::vector<std::shared_ptr<Asrt> > active_asrts_;
  static std::vector<std::shared_ptr<Asrt> > inactive_asrts_;
  static std::vector<std::shared_ptr<Asrt> > next_asrts_;

  static void reset();
  static void RegisterAsrt(std::shared_ptr<Asrt> asrt);
  static void Cleanup();

  static void SnoopBTransportBgn(const tlm::tlm_generic_payload& trans,
                                 const std::string& probe,
                                 const sc_core::sc_time& time);
  static void SnoopBTransportEnd(const tlm::tlm_generic_payload& trans,
                                 const std::string& probe,
                                 const sc_core::sc_time& time);
  static void SnoopNbTransportFwBgn(const tlm::tlm_generic_payload& trans,
                                    const std::string& probe,
                                    const sc_core::sc_time& time,
                                    const tlm::tlm_phase& phase);
  static void SnoopNbTransportFwEnd(const tlm::tlm_generic_payload& trans,
                                    const std::string& probe,
                                    const sc_core::sc_time& time,
                                    const tlm::tlm_phase& phase,
                                    const tlm::tlm_sync_enum& status);
  static void SnoopNbTransportBwBgn(const tlm::tlm_generic_payload& trans,
                                    const std::string& probe,
                                    const sc_core::sc_time& time,
                                    const tlm::tlm_phase& phase);
  static void SnoopNbTransportBwEnd(const tlm::tlm_generic_payload& trans,
                                    const std::string& probe,
                                    const sc_core::sc_time& time,
                                    const tlm::tlm_phase& phase,
                                    const tlm::tlm_sync_enum& status);

  static void RunEot();

  static std::string ToString();
};
}  // namespace Scasrt

#endif
