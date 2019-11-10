#include "doctest/doctest.h"
#include "std_e/utils/string.h"


TEST_CASE("Constant std::array") {
  std::string s = "abcde";

  std_e::remove_back_n(s,2);

  std::string expected_s = "abc";
  CHECK( s == expected_s );
}
