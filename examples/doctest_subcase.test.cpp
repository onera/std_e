#include "std_e/unit_test/doctest.hpp"
#include <iostream>

TEST_CASE("my test case") {
  std::cout << "common part at the beginning\n";
  SUBCASE("sub case 0") {
    std::cout << "part 0\n";
  }
  SUBCASE("sub case 1") {
    std::cout << "part 1\n";
  }
  std::cout << "common part at the end\n";
}
