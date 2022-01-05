#include "std_e/unit_test/doctest.hpp"
#include "std_e/algorithm/distribution/uniform_range.hpp"
#include <vector>

using namespace std_e;

TEST_CASE("uniform_distribution_range") {
  SUBCASE("empty remainder") {
    uniform_distribution_range<int> r(100,15,3);

    std::vector<int> v;
    for (int i : r) {
      v.push_back(i);
    }
  // distributing 15 over 3 slots gives
  //                             5 + 5 + 5
  //                             |   |   |
  //                             v   v   v
    CHECK( v == std::vector{100,105,110,115} );
  }

  SUBCASE("non-empty remainder") {
    uniform_distribution_range<int> r(100,17,3);

    std::vector<int> v;
    for (int i : r) {
      v.push_back(i);
    }
  // distributing 17 over 3 slots gives
  //                             6 + 6 + 5
  //                             |   |   |
  //                             v   v   v
    CHECK( v == std::vector{100,106,112,117} );
  }
}
