#if __cplusplus > 201703L
#include "std_e/unit_test/doctest.hpp"

#include "std_e/data_structure/block_range/vblock_range.hpp"
#include "std_e/future/ranges/algorithm.hpp"
#include <algorithm>

using namespace std_e;

TEST_CASE("vblock_range") {
  std::vector<double> x = {2.,10.,4.,5.,  6.,3.,12.};
  std::vector<int>   is = {0           ,  4        ,7};

  vblock_range<std_e::span<double>,std_e::span<int>> xb = view_as_vblock_range(x,is);

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
    vblock_range<std_e::span<double>,std_e::span<int>> yb = view_as_vblock_range(y,y_is);
    std_e::ranges::copy(xb,yb.begin());
    CHECK( x == std::vector{2.,10.,4.,5.,  6.,3.,12.} );
    CHECK( y == std::vector{2.,10.,4.,5.,  6.,3.,12.} );
    CHECK(   is == std::vector{0,4,7} );
    CHECK( y_is == std::vector{0,4,7} );
  }
  SUBCASE("move") {
    std::vector<double> y(7);
    std::vector<int> y_is(3);
    vblock_range<std_e::span<double>,std_e::span<int>> yb = view_as_vblock_range(y,y_is);
    std_e::ranges::move(xb,yb.begin());
    CHECK( y == std::vector{2.,10.,4.,5.,  6.,3.,12.} );
    CHECK( y_is == std::vector{0,4,7} );
  }
  SUBCASE("find") {
    auto it = std_e::ranges::find(xb,xb[1]);
    CHECK( *it == std::vector{6.,3.,12.} );
  }
  SUBCASE("find_if") {
    auto it = std_e::ranges::find_if(xb,[](auto&& x){ return x[0]==6.; });
    CHECK( *it == std::vector{6.,3.,12.} );
  }
  SUBCASE("any_of") {
    bool has_one_elt_begining_with_6 = std_e::ranges::any_of(xb,[](auto&& x){ return x[0]==6.; });
    CHECK( has_one_elt_begining_with_6 );
  }
  SUBCASE("partition_point") {
    auto it = std_e::ranges::partition_point(xb,[](auto&& x){ return x[0]==2.; });
    CHECK( *it == std::vector{6.,3.,12.} );
  }
  SUBCASE("lower_bound") {
    auto it = std_e::ranges::lower_bound(xb,xb[1]);
    CHECK( *it == std::vector{2.,10.,4.,5.} );
  }
  SUBCASE("unique") {
    std::vector<double> x = {2.,10.,4.,5.,  6.,3.,12.,  6.,3.,12., 44.,45.};
    std::vector<int>   is = {0           ,  4        ,  7        , 10     , 12};
    vblock_range<std_e::span<double>,std_e::span<int>> xb = view_as_vblock_range(x,is);
    auto sub_rng = std_e::ranges::unique(xb);
    CHECK( x == std::vector{2.,10.,4.,5.,  6.,3.,12.,  44.,45., 12.,44.,45.} ); // Note: 3 last values are from the old range
    CHECK( is == std::vector{0,4,7,9,12} ); // Note: last value from the old range
    CHECK( sub_rng.size() == 1 ); // One element duplicated
  }
}
#endif // C++>17
