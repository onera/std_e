#include "std_e/unit_test/doctest.hpp"
#include "std_e/multi_array/multi_array/strided_array.hpp"
#include "std_e/multi_array/multi_array.hpp"
#include "std_e/multi_index/multi_index_range.hpp"

using namespace std;
using namespace std_e;

TEST_CASE("origin indices") {
  SUBCASE("one index fixed") {
    multi_index<int,1> fixed_dim_indices = {1};
    multi_index<int,1> fixed_indices = {15};
    multi_index<int,2> indices = {10,20};

    multi_index<int,3> complete_indices = origin_indices(fixed_dim_indices,fixed_indices,indices);
    multi_index<int,3> expected_complete_indices = {10,15,20};
    CHECK( complete_indices == expected_complete_indices );
  }
  SUBCASE("two indices fixed") {
    multi_index<int,2> fixed_dim_indices = {0,2};
    multi_index<int,2> fixed_indices = {10,20};
    multi_index<int,1> indices = {15};

    multi_index<int,3> complete_indices = origin_indices(fixed_dim_indices,fixed_indices,indices);
    multi_index<int,3> expected_complete_indices = {10,15,20};
    CHECK( complete_indices == expected_complete_indices );
  }
}

// [Sphinx Doc] 2D array sub-views {
TEST_CASE("2D array sub-views") {
  dyn_multi_array<double,2> ma = {
    {1.,2.,3.},
    {4.,5.,6.}
  };
  SUBCASE("get columns") {
    auto col_0 = column(ma,0);
    CHECK( col_0[0] == 1. );
    CHECK( col_0[1] == 4. );
    auto col_1 = column(ma,1);
    CHECK( col_1[0] == 2. );
    CHECK( col_1[1] == 5. );
    auto col_2 = column(ma,2);
    CHECK( col_2[0] == 3. );
    CHECK( col_2[1] == 6. );
  }
  SUBCASE("get rows") {
    auto row_0 = row(ma,0);
    CHECK( row_0.size() == 3 );
    CHECK( row_0[0] == 1. );
    CHECK( row_0[1] == 2. );
    CHECK( row_0[2] == 3. );
    auto row_1 = row(ma,1);
    CHECK( row_1.size() == 3 );
    CHECK( row_1[0] == 4. );
    CHECK( row_1[1] == 5. );
    CHECK( row_1[2] == 6. );
  }
}
// [Sphinx Doc] 2D array sub-views }


// [Sphinx Doc] strided_array with one index {
TEST_CASE("strided_array with one index") {
  dyn_multi_array<double,3> ma(4,3,2);
  for (auto [i,j,k] : fortran_multi_index_range(ma.extent())) {
    ma(i,j,k) = 100*i + 10*j + k;
  }

  SUBCASE("compile time axis") {
    auto sub_ma_1_2 = make_strided_array<1>(ma,2); // fix axis 1 at index 2

    CHECK( sub_ma_1_2.rank() == 2 );
    CHECK( sub_ma_1_2.extent(0) == 4 );
    CHECK( sub_ma_1_2.extent(1) == 2 );

    CHECK( sub_ma_1_2(0,0) ==  20. );
    CHECK( sub_ma_1_2(1,0) == 120. );
    CHECK( sub_ma_1_2(2,0) == 220. );
    CHECK( sub_ma_1_2(3,0) == 320. );
    CHECK( sub_ma_1_2(0,1) ==  21. );
    CHECK( sub_ma_1_2(1,1) == 121. );
    CHECK( sub_ma_1_2(2,1) == 221. );
    CHECK( sub_ma_1_2(3,1) == 321. );
                          //   ^
                          // fixed
  }
  SUBCASE("run time axis") {
    auto sub_ma_1_2 = make_strided_array(ma,1,2); // fix axis 1 at index 2

    CHECK( sub_ma_1_2.rank() == 2 );
    CHECK( sub_ma_1_2.extent(0) == 4 );
    CHECK( sub_ma_1_2.extent(1) == 2 );

    CHECK( sub_ma_1_2(0,0) ==  20. );
    CHECK( sub_ma_1_2(1,0) == 120. );
    CHECK( sub_ma_1_2(2,0) == 220. );
    CHECK( sub_ma_1_2(3,0) == 320. );
    CHECK( sub_ma_1_2(0,1) ==  21. );
    CHECK( sub_ma_1_2(1,1) == 121. );
    CHECK( sub_ma_1_2(2,1) == 221. );
    CHECK( sub_ma_1_2(3,1) == 321. );
  }
}
// [Sphinx Doc] strided_array with one index }


// [Sphinx Doc] strided_array with multiple indices {
TEST_CASE("strided_array with multiple indices") {
  dyn_multi_array<double,4> ma(5,4,3,2);
  for (auto [i,j,k,l] : fortran_multi_index_range(ma.extent())) {
    ma(i,j,k,l) = 1000*i + 100*j + 10*k + l;
  }

  SUBCASE("resulting sub-array is multi-dimensional") {
    SUBCASE("compile time axis") {
                           // fix axes 0 and 2...
                           //           \   /
                           //            | | ... at respective indices
                           //            | |    3 and 1
                           //            | |     \   /
                           //            | |      | |
                           //            v v      v v
      auto ma_02_31 = make_strided_array<0,2>(ma,{3,1});
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
                          //  ^ ^
                          // fixed
    }
    SUBCASE("run time axis") {
      auto ma_02_31 = make_strided_array(ma,{0,2},{3,1}); // fix axes 0 and 2 at indices 3 and 1
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
  }

  SUBCASE("special case: only one direction not fixed, so the resulting sub-array is 1D") {
    auto ma_012_320 = make_strided_span<1>(ma,{3,2,1}); // fix all except axis 1, at indices 3,2 and 1
    CHECK( ma_012_320.size() == 4 );
    CHECK( ma_012_320.stride_length() == 5 );
    CHECK( ma_012_320.data() - ma.data() == 3 + 5*0 + (5*4)*2 + (5*4*3)*1 );
    CHECK( ma_012_320[0] == 3021. );
    CHECK( ma_012_320[1] == 3121. );
    CHECK( ma_012_320[2] == 3221. );
    CHECK( ma_012_320[3] == 3321. );
                        //  ^ ^^
                        // fixed
  }
}
// [Sphinx Doc] strided_array with multiple indices }
