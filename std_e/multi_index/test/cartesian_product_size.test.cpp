#include "std_e/unit_test/doctest.hpp"
#include "std_e/multi_index/cartesian_product_size.hpp"
#include "std_e/multi_index/multi_index.hpp"

using namespace std_e;


TEST_CASE("Cartesian product size") {
  multi_index<int,2> is0 = {2,3};
  CHECK( std_e::cartesian_product_size(is0) == 2*3 );

  multi_index<int,3> is1 = {8,7,6};
  CHECK( std_e::cartesian_product_size(is1) == 8*7*6 );

  multi_index<int,0> is2 = {};
  CHECK( std_e::cartesian_product_size(is2) == 1 );
}
