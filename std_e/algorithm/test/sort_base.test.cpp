#include "std_e/unit_test/doctest.hpp"

#include "std_e/algorithm/sort_base.hpp"
using std::vector;

TEST_CASE("sorted") {
  CHECK( std_e::sorted(vector<int>{4,3,1,5,2}) == vector<int>{1,2,3,4,5} );
}
