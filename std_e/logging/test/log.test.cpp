#include "std_e/unit_test/doctest.hpp"
#include "std_e/logging/log.hpp"
#include "std_e/logging/printer_common.hpp"

using namespace std_e;

TEST_CASE("log") {
  add_logger_if_absent(logger{"logger_for_unit_tests"});

  // at this point, no printer is associated to the logger
  log("logger_for_unit_tests", "msg 0|"); // does nothing 

  // associate a printer
  std::string s0;
  add_printer_to_logger("logger_for_unit_tests", recording_printer{&s0});

  log("logger_for_unit_tests", "msg 1|");
  CHECK( s0 == "msg 1|" );

  // turn off the logger
  turn_off("logger_for_unit_tests");
  log("logger_for_unit_tests", "msg 2|");
  CHECK( s0 == "msg 1|" );

  // turn on the logger
  turn_on("logger_for_unit_tests");
  log("logger_for_unit_tests", "msg 3|");
  CHECK( s0 == "msg 1|msg 3|" );

  // associate a second printer
  std::string s1;
  add_printer_to_logger("logger_for_unit_tests", recording_printer{&s1});

  log("logger_for_unit_tests", "msg 4|");
  CHECK( s0 == "msg 1|msg 3|msg 4|" );
  CHECK( s1 == "msg 4|" );
}
