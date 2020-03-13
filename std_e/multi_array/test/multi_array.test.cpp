#include "doctest/doctest.h"
#include "std_e/multi_array/multi_array.hpp"


using namespace std;
using namespace std_e;


TEST_CASE("multi_arrays of rank 0 are scalars") {
  SUBCASE("fixed array") {
    fixed_multi_array<int> f;
    CHECK( f.rank() == 0 );
    CHECK( f.size() == 1 );

    f() = 42;
    CHECK( f() == 42 );
  }
  SUBCASE("dyn array") {
    // The handling of dyn arrays of rank 0 is not well supported
    // because it would require special cases
    // whereas it seems not very useful

    dyn_multi_array<int,int32_t,0> f_no_alloc;
    CHECK( f_no_alloc.rank() == 0 );
    CHECK( f_no_alloc.size() == 1 ); 

    // WARNING: nothing has been allocated!
    // The following would segfault
    // f_no_alloc() = 42; // BAD: SEGFAULT
  }
}


TEST_CASE("dyn_multi_array_view") {
  vector<int> v = {1,2,3,4,5,6};
  std_e::memory_view<int*> external_memory(v.data());
  multi_index<int32_t,2> shape_dims = {3,2};
  
  dyn_multi_array_view<int,int32_t,2> mav = {external_memory,{shape_dims}};

  SUBCASE("basic_tests") {

    CHECK( mav.size() == 6);
    CHECK( mav.extent(0) == 3);
    CHECK( mav.extent(1) == 2);

    CHECK( mav(0,0) == 1 );
    CHECK( mav(1,0) == 2 );
    CHECK( mav(2,0) == 3 );
    CHECK( mav(0,1) == 4 );
    CHECK( mav(1,1) == 5 );
    CHECK( mav(2,1) == 6 );
  }

  SUBCASE("using_external_memory") {
    mav(0,0) = 10;
    CHECK( v[0] == 10 );
    CHECK( v[1] ==  2 );
    CHECK( v[2] ==  3 );
    CHECK( v[3] ==  4 );
    CHECK( v[4] ==  5 );
    CHECK( v[5] ==  6 );
  }
}


TEST_CASE("dyn_multi_array") {
  dyn_multi_array<int,int32_t,2> ma = {
    {1,2,3},
    {4,5,6}
  };

  CHECK( ma.size() == 6 );
  CHECK( ma.extent(0) == 2 );
  CHECK( ma.extent(1) == 3 );

  CHECK( ma(0,0) == 1 ); CHECK( ma(0,1) == 2 ); CHECK( ma(0,2) == 3 );
  CHECK( ma(1,0) == 4 ); CHECK( ma(1,1) == 5 ); CHECK( ma(1,2) == 6 );
}


TEST_CASE("dyn_multi_array equality") {
  vector<int> v = {1,2,3,4,5,6};
  memory_view<int*> external_memory(v.data());
  multi_index<int32_t,2> shape_dims = {3,2};
  
  dyn_multi_array_view<int,int32_t,2> x = {external_memory,{shape_dims}};

  dyn_multi_array<int,int32_t,2> y({1,2,3,4,5,6},{shape_dims});
  dyn_multi_array<int,int32_t,2> z({1,2,3,4,0,6},{shape_dims});

  CHECK(x==y);
  CHECK(x!=z);
}


TEST_CASE("make_sub_view") {
  SUBCASE("one index") {
    dyn_multi_array<int,int32_t,2> ma = {
      {1,2,3},
      {4,5,6}
    };

    auto sub_ma_0 = make_sub_view(ma,0);
    CHECK( sub_ma_0(0) == 1 );
    CHECK( sub_ma_0(1) == 4 );
    auto sub_ma_1 = make_sub_view(ma,1);
    CHECK( sub_ma_1(0) == 2 );
    CHECK( sub_ma_1(1) == 5 );
    auto sub_ma_2 = make_sub_view(ma,2);
    CHECK( sub_ma_2(0) == 3 );
    CHECK( sub_ma_2(1) == 6 );
  }
  SUBCASE("multi-index") {
    dyn_multi_array<double,int32_t,3> ma(4,3,2);
    ma(0,0,0) =   0.;
    ma(1,0,0) = 100.;
    ma(2,0,0) = 200.;
    ma(3,0,0) = 300.;
    ma(0,1,0) =  10.;
    ma(1,1,0) = 110.;
    ma(2,1,0) = 210.;
    ma(3,1,0) = 310.;
    ma(0,2,0) =  20.;
    ma(1,2,0) = 120.;
    ma(2,2,0) = 220.;
    ma(3,2,0) = 320.;
    ma(0,0,1) =   1.;
    ma(1,0,1) = 101.;
    ma(2,0,1) = 201.;
    ma(3,0,1) = 301.;
    ma(0,1,1) =  11.;
    ma(1,1,1) = 111.;
    ma(2,1,1) = 211.;
    ma(3,1,1) = 311.;
    ma(0,2,1) =  21.;
    ma(1,2,1) = 121.;
    ma(2,2,1) = 221.;
    ma(3,2,1) = 321.;

    auto sub_ma_00 = make_sub_view(ma,std_e::multi_index<int,2>{0,0});
    CHECK( sub_ma_00(0) ==   0. );
    CHECK( sub_ma_00(1) == 100. );
    CHECK( sub_ma_00(2) == 200. );
    CHECK( sub_ma_00(3) == 300. );
    auto sub_ma_11 = make_sub_view(ma,std_e::multi_index<int,2>{1,1});
    CHECK( sub_ma_11(0) ==  11. );
    CHECK( sub_ma_11(1) == 111. );
    CHECK( sub_ma_11(2) == 211. );
    CHECK( sub_ma_11(3) == 311. );
  }
}

TEST_CASE("make_span") {
  SUBCASE("one index") {
    dyn_multi_array<int,int32_t,2> ma = {
      {1,2,3},
      {4,5,6}
    };

    auto span_0 = make_span(ma,0);
    CHECK( span_0[0] == 1 );
    CHECK( span_0[1] == 4 );
    auto span_1 = make_span(ma,1);
    CHECK( span_1[0] == 2 );
    CHECK( span_1[1] == 5 );
    auto span_2 = make_span(ma,2);
    CHECK( span_2[0] == 3 );
    CHECK( span_2[1] == 6 );
  }
}
