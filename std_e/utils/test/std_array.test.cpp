#include "std_e/unit_test/doctest.hpp"
#include "std_e/utils/std_array.hpp"

TEST_CASE("std::array sub_array") {
  std::array<int,6> a              = {3,4,5,6,7,8};
  std::array<int,3> expected_sub_a =     {5,6,7};
  CHECK( std_e::make_sub_array<2,3>(a) == expected_sub_a );
}
