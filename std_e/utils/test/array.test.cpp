#include "std_e/unit_test/doctest.hpp"
#include "std_e/utils/array.hpp"


TEST_CASE("Constant std_e::array") {
  std_e::array<int,4> a = std_e::constant_array<4>(42);
  std_e::array<int,4> expected_a = {42,42,42,42};
  CHECK( a == expected_a );
}

TEST_CASE("Default std_e::array") {
  std_e::array<int,4> a = std_e::default_array<int,4>;
  std_e::array<int,4> expected_a = {0,0,0,0};
  CHECK( a == expected_a );
}

TEST_CASE("std_e::array sub_array") {
  std_e::array<int,6> a              = {3,4,5,6,7,8};
  std_e::array<int,3> expected_sub_a =     {5,6,7};
  CHECK( std_e::make_sub_array<2,3>(a) == expected_sub_a );
}

TEST_CASE("Default std_e::array except for one index") {
  std_e::array<int,4> a = std_e::default_array_except<4>(2,42);
  std_e::array<int,4> expected_a = {0,0,42,0};
  CHECK( a == expected_a );
}
