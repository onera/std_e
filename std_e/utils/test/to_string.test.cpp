#include "doctest/doctest.h"

#include "std_e/utils/to_string.hpp"
#include <vector>

using namespace std_e;

TEST_CASE("range_to_string") {
  std::vector<int> v0 = {};
  CHECK( range_to_string(v0)==std::string("{}") );

  std::vector<int> v1 = {42};
  CHECK( range_to_string(v1)==std::string("{42}") );

  std::vector<int> v2 = {42,43,44};
  CHECK( range_to_string(v2)==std::string("{42;43;44}") );
}