#include "std_e/unit_test/doctest.hpp"
#include "std_e/future/algorithm.hpp"

TEST_CASE("inclusive and exclusive scan") {
  std::vector<int> data = {3, 1, 4, 1, 5, 9, 2, 6};

  SUBCASE("inclusive") {
    std::vector<int> res;
    std_e::inclusive_scan(data.begin(), data.end(), std::back_inserter(res));
    CHECK( res == std::vector{3,4,8,9,14,23,25,31} );
  }
  SUBCASE("exclusive") {
    std::vector<int> res;
    std_e::exclusive_scan(data.begin(), data.end(), std::back_inserter(res), 0);
    CHECK( res == std::vector{0,3,4,8,9,14,23,25} );
  }
}

