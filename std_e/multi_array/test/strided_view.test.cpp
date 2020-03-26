#include "doctest/doctest.h"
#include "std_e/multi_array/multi_array.hpp"
#include "std_e/multi_array/multi_array.hpp"


using namespace std;
using namespace std_e;


TEST_CASE("strided_view") {
  //SUBCASE("one index") {
  //  dyn_multi_array<int,int32_t,2> ma = {
  //    {1,2,3},
  //    {4,5,6}
  //  };

  //  auto sub_ma_0 = make_sub_view(ma,0);
  //  CHECK( sub_ma_0(0) == 1 );
  //  CHECK( sub_ma_0(1) == 4 );
  //  auto sub_ma_1 = make_sub_view(ma,1);
  //  CHECK( sub_ma_1(0) == 2 );
  //  CHECK( sub_ma_1(1) == 5 );
  //  auto sub_ma_2 = make_sub_view(ma,2);
  //  CHECK( sub_ma_2(0) == 3 );
  //  CHECK( sub_ma_2(1) == 6 );
  //}
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
