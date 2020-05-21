#include "std_e/unit_test/doctest.hpp"
#include "std_e/multi_array/multi_array/strided_array.hpp"
#include "std_e/multi_array/multi_array.hpp"
#include "std_e/multi_index/multi_index_range.hpp"

using namespace std;
using namespace std_e;

TEST_CASE("strided_array") {
  dyn_multi_array<double,4> ma(5,4,3,2);
  for (const auto& is : fortran_multi_index_range(ma.extent())) {
    ma(is) = 1000*is[0] + 100*is[1] + 10*is[2] + is[3];
  }

  SUBCASE("run time fixed positions") {
    auto ma_02_31 = make_strided_array(ma,multi_index<int,2>{0,2},multi_index<int,2>{3,1}); // fix extent 0 and 2 at positions 3 and 1
    CHECK( ma_02_31.rank() == 2 );
    CHECK( ma_02_31.extent(0) == 4 );
    CHECK( ma_02_31.extent(1) == 2 );
    CHECK( ma_02_31(0,0) == 3010. );
    CHECK( ma_02_31(1,0) == 3110. );
    CHECK( ma_02_31(2,0) == 3210. );
    CHECK( ma_02_31(3,0) == 3310. );
    CHECK( ma_02_31(0,1) == 3011. );
    CHECK( ma_02_31(1,1) == 3111. );
    CHECK( ma_02_31(2,1) == 3211. );
    CHECK( ma_02_31(3,1) == 3311. );
  }
  SUBCASE("compile time fixed positions") {
    auto ma_02_31 = make_strided_array<0,2>(ma,multi_index<int,2>{3,1}); // fix extent 0 and 2 at positions 3 and 1
    CHECK( ma_02_31.rank() == 2 );
    CHECK( ma_02_31.extent(0) == 4 );
    CHECK( ma_02_31.extent(1) == 2 );
    CHECK( ma_02_31(0,0) == 3010. );
    CHECK( ma_02_31(1,0) == 3110. );
    CHECK( ma_02_31(2,0) == 3210. );
    CHECK( ma_02_31(3,0) == 3310. );
    CHECK( ma_02_31(0,1) == 3011. );
    CHECK( ma_02_31(1,1) == 3111. );
    CHECK( ma_02_31(2,1) == 3211. );
    CHECK( ma_02_31(3,1) == 3311. );
  }

  SUBCASE("ct fixed position span") {
    auto ma_012_320 = make_strided_span<1>(ma,multi_index<int,3>{3,2,1}); // fix all except direction 1, at positions 3,2 and 1
    CHECK( ma_012_320.size() == 4 );
    CHECK( ma_012_320.stride_length() == 5 );
    CHECK( ma_012_320.data() - ma.data() == 3 + 5*0 + (5*4)*2 + (5*4*3)*1 );
    CHECK( ma_012_320[0] == 3021. );
    CHECK( ma_012_320[1] == 3121. );
    CHECK( ma_012_320[2] == 3221. );
    CHECK( ma_012_320[3] == 3321. );
  }
}


TEST_CASE("strided_array with only one index") {
  dyn_multi_array<int,2> ma = {
    {1,2,3},
    {4,5,6}
  };

  SUBCASE("run time fixed positions") {
    auto sub_ma_0_0 = make_strided_array(ma,0,0);
    CHECK( sub_ma_0_0(0) == 1 );
    CHECK( sub_ma_0_0(1) == 2 );
    CHECK( sub_ma_0_0(2) == 3 );
    auto sub_ma_0_1 = make_strided_array(ma,0,1);
    CHECK( sub_ma_0_1(0) == 4 );
    CHECK( sub_ma_0_1(1) == 5 );
    CHECK( sub_ma_0_1(2) == 6 );

    auto sub_ma_1_0 = make_strided_array(ma,1,0);
    CHECK( sub_ma_1_0(0) == 1 );
    CHECK( sub_ma_1_0(1) == 4 );
    auto sub_ma_1_1 = make_strided_array(ma,1,1);
    CHECK( sub_ma_1_1(0) == 2 );
    CHECK( sub_ma_1_1(1) == 5 );
    auto sub_ma_1_2 = make_strided_array(ma,1,2);
    CHECK( sub_ma_1_2(0) == 3 );
    CHECK( sub_ma_1_2(1) == 6 );
  }
  SUBCASE("compile time fixed positions") {
    auto sub_ma_0_0 = make_strided_array<0>(ma,0);
    CHECK( sub_ma_0_0(0) == 1 );
    CHECK( sub_ma_0_0(1) == 2 );
    CHECK( sub_ma_0_0(2) == 3 );
    auto sub_ma_0_1 = make_strided_array<0>(ma,1);
    CHECK( sub_ma_0_1(0) == 4 );
    CHECK( sub_ma_0_1(1) == 5 );
    CHECK( sub_ma_0_1(2) == 6 );

    auto sub_ma_1_0 = make_strided_array<1>(ma,0);
    CHECK( sub_ma_1_0(0) == 1 );
    CHECK( sub_ma_1_0(1) == 4 );
    auto sub_ma_1_1 = make_strided_array<1>(ma,1);
    CHECK( sub_ma_1_1(0) == 2 );
    CHECK( sub_ma_1_1(1) == 5 );
    auto sub_ma_1_2 = make_strided_array<1>(ma,2);
    CHECK( sub_ma_1_2(0) == 3 );
    CHECK( sub_ma_1_2(1) == 6 );
  }

  SUBCASE("strided_span") {
    // make_strided_span for 2D multi arrays is a special case and does not have exactly the same interface as in the general case
    // For 2D arrays, we either want to
    //   fix the last direction, and for that we can ask for a column with make_span, which is contiguous
    //   for the first direction, and for that we can ask for a row with make_strided_span, which is non-contiguous
    SUBCASE("get columns") {
      auto span_0 = column(ma,0);
      CHECK( span_0[0] == 1 );
      CHECK( span_0[1] == 4 );
      auto span_1 = column(ma,1);
      CHECK( span_1[0] == 2 );
      CHECK( span_1[1] == 5 );
      auto span_2 = column(ma,2);
      CHECK( span_2[0] == 3 );
      CHECK( span_2[1] == 6 );
    }

    SUBCASE("get rows") {
      auto sub_ma_0_0 = row(ma,0);
      CHECK( sub_ma_0_0.size() == 3 );
      CHECK( sub_ma_0_0[0] == 1 );
      CHECK( sub_ma_0_0[1] == 2 );
      CHECK( sub_ma_0_0[2] == 3 );
      auto sub_ma_0_1 = row(ma,1);
      CHECK( sub_ma_0_1.size() == 3 );
      CHECK( sub_ma_0_1[0] == 4 );
      CHECK( sub_ma_0_1[1] == 5 );
      CHECK( sub_ma_0_1[2] == 6 );
    }
  }
}
