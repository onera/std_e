#include "std_e/unit_test/doctest.hpp"

#include "std_e/utils/to_string.hpp"
#include <vector>

using namespace std_e;

TEST_CASE("to_string(std::string)") {
  // NOTE: only useful for genericity of to_string()
  std::string s = "abc";
  CHECK( std_e::to_string(s) == "abc" );
}

TEST_CASE("to_string(ptr)") {
  int* ptr = nullptr;
  CHECK( std_e::to_string(ptr) == "0" );
}

TEST_CASE("range_to_string") {
  std::vector<int> v0 = {};
  CHECK( range_to_string(v0)==std::string("{}") );

  std::vector<int> v1 = {42};
  CHECK( range_to_string(v1)==std::string("{42}") );

  std::vector<int> v2 = {42,43,44};
  CHECK( range_to_string(v2)==std::string("{42;43;44}") );
}
