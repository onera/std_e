#include "std_e/unit_test/doctest.hpp"
#include "std_e/multi_index/cartesian_product.hpp"
#include "std_e/multi_index/multi_index.hpp"

using namespace std_e;


TEST_CASE("Cartesian product") {
  multi_index<int> is0 = {2,3};
  CHECK( std_e::cartesian_product(is0) == 2*3 );

  multi_index<int> is1 = {8,7,6};
  CHECK( std_e::cartesian_product(is1) == 8*7*6 );

  multi_index<int> is2 = {};
  CHECK( std_e::cartesian_product(is2) == 1 );
}
