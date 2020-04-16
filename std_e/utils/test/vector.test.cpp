#include "std_e/unit_test/doctest.hpp"
#include "std_e/utils/vector.hpp"


TEST_CASE("sort_unique(vector)") {
  std::vector<int> v = {9,1,1,4,3,1,9,8,4,7};

  std_e::sort_unique(v);

  std::vector<int> expected_v = {1,3,4,7,8,9};
  CHECK( v == expected_v );
}

TEST_CASE("std::vector sub_vector") {
  std::vector<int> a              = {3,4,5,6,7,8};
  std::vector<int> expected_sub_a =     {5,6,7};
  CHECK( std_e::make_sub_vector(a,2,3) == expected_sub_a );
}
