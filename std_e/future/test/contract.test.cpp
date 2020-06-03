#include "std_e/unit_test/doctest.hpp"
#include "std_e/future/contract.hpp"

TEST_CASE("assertion_failure") {
  std_e::assertion_failure assert_fail("my_file.hpp",10,"sz > 0");

  std::string desc = assert_fail.what();
  std::string expected_desc = "assertion failure at file my_file.hpp, line 10\n"
                              "failed assertion: sz > 0";
  CHECK( desc == expected_desc );
}
