#include "std_e/unit_test/doctest.hpp"

#include "std_e/data_structure/block_range/block_range.hpp"
#include "std_e/future/sort/sort_ranges.hpp"
#include <algorithm>

using namespace std_e;

TEST_CASE("block_range") {
  std::vector<int> x = {10,4,5,  6,3,12};

  block_range<std::vector<int>,3> xb = view_as_block_range<3>(x);

  SUBCASE("basic methods") {
    CHECK( xb.total_size() == 6 );
    CHECK( xb.size() == 2 );

    CHECK( xb.block_size() == 3 );
    static_assert( block_range<std::vector<int>,3>::block_size() == 3 ); // same thing, but as static method
  }

  SUBCASE("operator[]") {
    CHECK( xb[0] == std::vector{10, 4, 5} );
    CHECK( xb[1] == std::vector{ 6, 3,12} );
  }
  SUBCASE("assignement") {
    xb[0][1] = 99;
    xb[1][2] = 0;
    CHECK( xb[0] == std::vector{10,99, 5} );
    CHECK( xb[1] == std::vector{ 6, 3, 0} );
  }

  SUBCASE("reverse") {
    std::ranges::reverse(xb);
    CHECK( x == std::vector{6,3,12,  10,4,5} );
  }
  SUBCASE("partition") {
    std::ranges::partition(xb,[](const auto& e){ return e[0] < 7; });
    CHECK( x == std::vector{6,3,12,  10,4,5} );
  }
  SUBCASE("sort") {
    // Note: As of GCC 11, std::ranges::sort is broken for proxy references
    //     see https://gcc.gnu.org/bugzilla/show_bug.cgi?id=104561
    //std::ranges::sort(begin(xb),end(xb));

    std_e::quicksort(begin(xb),end(xb));
    CHECK( x == std::vector{6,3,12,  10,4,5} );
  }
};
