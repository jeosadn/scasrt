#ifdef RUN_UNIT_TESTS
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#else
#define DOCTEST_CONFIG_DISABLE
#endif
#include "doctest/doctest.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

auto scasrt_log = spdlog::stdout_color_st("scasrt_log");
auto scasrt_test_log = spdlog::stdout_color_st("scasrt_test_log");

#include "../src/Asrt.cpp"
#include "../src/Kernel.cpp"
#include "../src/Prop.cpp"
#include "../src/Tlm.cpp"
#include "../src/Types.cpp"
#include "../tst/Asrt.cpp"
#include "../tst/Kernel.cpp"
#include "../tst/Prop.cpp"

int sc_main(int argc, char* argv[]) { return 0; }
