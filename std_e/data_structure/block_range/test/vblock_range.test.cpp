#include "std_e/unit_test/doctest.hpp"

#include "std_e/data_structure/block_range/vblock_range.hpp"
#include <algorithm>

using namespace std_e;

TEST_CASE("block_range") {
  std::vector<double> x = {2.,10.,4.,5.,  6.,3.,12.};
  std::vector<int>   is = {0           ,  4        ,7};

  vblock_range<std::vector<double>&,std::vector<int>&> xb = view_as_vblock_range(x,is);

  SUBCASE("basic methods") {
    CHECK( xb.total_size() == 7 );
    CHECK( xb.size() == 2 );
  }

  SUBCASE("operator[]") {
    CHECK( xb[0] == std::vector{2.,10.,4.,5.} );
    CHECK( xb[1] == std::vector{6.,3.,12.} );
  }
  SUBCASE("assignement") {
    xb[0][1] = 99;
    xb[1][2] = 0;
    CHECK( xb[0] == std::vector{2.,99.,4.,5.} );
    CHECK( xb[1] == std::vector{6.,3.,0.} );
    CHECK( x == std::vector{2.,99.,4.,5.,  6.,3.,0.} );
  }

  SUBCASE("copy") {
    std::vector<double> y(7);
    std::vector<int> y_is(3);
    vblock_range<std::vector<double>&,std::vector<int>&> yb = view_as_vblock_range(y,y_is);
    std::ranges::copy(xb,yb.begin());
    CHECK( x == std::vector{2.,10.,4.,5.,  6.,3.,12.} );
    CHECK( y == std::vector{2.,10.,4.,5.,  6.,3.,12.} );
    CHECK(   is == std::vector{0,4,7} );
    CHECK( y_is == std::vector{0,4,7} );
  }
  SUBCASE("move") {
    std::vector<double> y(7);
    std::vector<int> y_is(3);
    vblock_range<std::vector<double>&,std::vector<int>&> yb = view_as_vblock_range(y,y_is);
    std::ranges::move(xb,yb.begin());
    CHECK( y == std::vector{2.,10.,4.,5.,  6.,3.,12.} );
    CHECK( y_is == std::vector{0,4,7} );
  }
}
