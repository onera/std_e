#include "std_e/unit_test/doctest.hpp"
#include "std_e/utils/array.hpp"

TEST_CASE("same_array_type_except_size") {
  using type = std_e::same_array_type_except_size< std::array<int,3> , 4>;
  using expected_type =                            std::array<int,     4>;
  static_assert( std::is_same_v<type,expected_type> );
}
