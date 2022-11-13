#if __cplusplus > 201703L
#include "std_e/unit_test/doctest.hpp"

#include "std_e/data_structure/block_range/ivblock_range.hpp"
#include "std_e/future/ranges/algorithm.hpp"
#include <algorithm>

using namespace std_e;

TEST_CASE("ivblock_range") {
  std::vector<int> x = {4,  2,10,4,5,    3,  6,3,12};

  ivblock_range<std::vector<int>&> xb = view_as_ivblock_range(x);

  SUBCASE("basic methods") {
    CHECK( xb.total_size() == 9 );
  }

  SUBCASE("copy") {
    std::vector<int> y(9);
    ivblock_range<std::vector<int>&> yb = view_as_ivblock_range(y);
    std_e::ranges::copy(xb,yb.begin());
    CHECK( x == std::vector{4,  2,10,4,5,    3,  6,3,12} );
    CHECK( y == std::vector{4,  2,10,4,5,    3,  6,3,12} );
  }
  SUBCASE("move") {
    std::vector<int> y(9);
    ivblock_range<std::vector<int>&> yb = view_as_ivblock_range(y);
    std_e::ranges::move(xb,yb.begin());
    CHECK( y == std::vector{4,  2,10,4,5,    3,  6,3,12} );
  }
}
#endif // C++>17
