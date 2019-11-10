#include "doctest/doctest.hpp"
#include "std_e/utils/string.hpp"


TEST_CASE("Constant std::array") {
  std::string s = "abcde";

  std_e::remove_back_n(s,2);

  std::string expected_s = "abc";
  CHECK( s == expected_s );
}
