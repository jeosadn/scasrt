#include "Utils.h"

std::shared_ptr<const Scasrt::Tlm> make_tlm(const sc_dt::uint64& address,
                                            const std::string& probe,
                                            const sc_core::sc_time& time) {
  tlm::tlm_generic_payload trans;

  trans.set_address(address);
  unsigned char data[4] = {'A', 'B', 'C', 'D'};
  trans.set_data_ptr((unsigned char*)&data);
  auto tlm = std::make_shared<const Scasrt::Tlm>(trans, probe, time);

  return tlm;
}

void setup_testcase() {
  Scasrt::Kernel::GetInstance()->reset();

  spdlog::get("scasrt_log")
      ->set_pattern("[%-12!n] [%L] [%-10!s] [%-15!!] [%-3!#] %v");
  spdlog::get("scasrt_test_log")
      ->set_pattern("[%-12!n] [%L] [%-10!s] [               ] [%-3!#] %v");
}
