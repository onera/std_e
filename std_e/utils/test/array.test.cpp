#include "std_e/unit_test/doctest.hpp"
#include "std_e/utils/array.hpp"

TEST_CASE("same_array_type_except_size") {
  using type = std_e::same_array_type_except_size< std::array<int,3> , 4>;
  using expected_type =                            std::array<int,     4>;
  static_assert( std::is_same_v<type,expected_type> );
}

TEST_CASE("Constant std::array") {
  auto a = std_e::constant_array<std::array<int,4>>(42);
  std::array<int,4> expected_a = {42,42,42,42};
  CHECK( a == expected_a );
}

TEST_CASE("Default std::array") {
  auto a = std_e::default_array<std::array<int,4>>;
  std::array<int,4> expected_a = {0,0,0,0};
  CHECK( a == expected_a );
}

TEST_CASE("Default std::array except for one index") {
  auto a = std_e::default_array_except<std::array<int,4>>(2,42);
  std::array<int,4> expected_a = {0,0,42,0};
  CHECK( a == expected_a );
}
