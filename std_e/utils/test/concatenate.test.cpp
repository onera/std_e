#include "std_e/unit_test/doctest.hpp"
#include "std_e/utils/concatenate.hpp"
#include <vector>

TEST_CASE("Concatenate vectors") {
  std::vector<int> v0 = {1,2,3,9};
  std::vector<int> v1 = {8,7,6};
  std::vector<int> v2 = {10,14};

  std::vector<int> v012 = std_e::concatenate(v0,v1,v2);
  std::vector<int> expected_v012 = {1,2,3,9,8,7,6,10,14};
  CHECK( v012 == expected_v012 );
}
TEST_CASE("Concatenate arrays") {
  std::array<int,4> v0 = {1,2,3,9};
  std::array<int,3> v1 = {8,7,6};
  std::array<int,2> v2 = {10,14};

  std::array<int,4+3+2> v012 = std_e::concatenate(v0,v1,v2);
  std::array<int,4+3+2> expected_v012 = {1,2,3,9,8,7,6,10,14};
  CHECK( v012 == expected_v012 );
}
