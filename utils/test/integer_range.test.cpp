#include "doctest/doctest.hpp"
#include "std_e/utils/integer_range.hpp"

TEST_CASE("integer_range") {
  using irange = std_e::integer_range<int>;

  irange r = {3,6};
  CHECK( std_e::length(r) == 3 );
  CHECK( !std_e::in(2,r) );
  CHECK(  std_e::in(3,r) );
  CHECK(  std_e::in(4,r) );
  CHECK(  std_e::in(5,r) );
  CHECK( !std_e::in(6,r) );
}

