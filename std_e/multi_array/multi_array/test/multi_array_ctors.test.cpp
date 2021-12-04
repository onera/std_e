#include "std_e/unit_test/doctest.hpp"
#include "std_e/multi_array/multi_array/multi_array_types.hpp"
#include "std_e/multi_array/utils.hpp"

using namespace std_e;

// [Sphinx Doc] multi_array constructors - from extent {
TEST_CASE("multi_array constructors - from extent") {


  SUBCASE("default constructor") {
    SUBCASE("fixed size") {
      fixed_multi_array<double,3,2> ma;
      CHECK( ma.extent() == multi_index<int,2>{3,2} );
    }

    SUBCASE("dynamic size") {
      dyn_multi_array<double,2> ma;
      CHECK( ma.extent() == zero_multi_index<int,2> );

      reshape(ma,{3,2});
      CHECK( ma.extent() == multi_index<int,2>{3,2} );
    }
  }


  SUBCASE("from extent") {
    dyn_multi_array<double,2> ma(3,2);
    CHECK( ma.extent() == multi_index<int,2>{3,2} );
  }


  SUBCASE("for views: from pointer to memory and extent") {
    std::vector<double> v = {0.,1.,2.,3.,4.,5.};

    dyn_multi_array_view<double,2> ma(v.data(),{3,2});

    CHECK( ma(0,0) == 0. ); CHECK( ma(0,1) == 3. );
    CHECK( ma(1,0) == 1. ); CHECK( ma(1,1) == 4. );
    CHECK( ma(2,0) == 2. ); CHECK( ma(2,1) == 5. );
  }
}
// [Sphinx Doc] multi_array constructors - from extent }

// [Sphinx Doc] multi_array constructors - init lists {
TEST_CASE("multi_array constructors - from initialization lists (only 1D and 2D)") {

  SUBCASE("for multi-array containers") {

    SUBCASE("1D") {
      dyn_multi_array<double,1> ma = {0.,1.,2.};
      CHECK( ma.extent(0) == 3 );
    }

    SUBCASE("2D") {
      dyn_multi_array<double,2> ma0 = {{0.,1.},{2.,3.},{4.,5.}};
      CHECK( ma0.extent(0) == 3 );
      CHECK( ma0.extent(1) == 2 );

      dyn_multi_array<double,2> ma1 = {{0.,1.,3.}};
      CHECK( ma1.extent(0) == 1 );
      CHECK( ma1.extent(1) == 3 );

      dyn_multi_array<double,2> ma2 = {{0.},{1.},{3.}};
      CHECK( ma2.extent(0) == 3 );
      CHECK( ma2.extent(1) == 1 );
    }
  }


  SUBCASE("for multi-array views") {

    SUBCASE("1D") {
      std::vector<double> v(3);
      dyn_multi_array_view<double,1> ma = {{0,1,2},v.data()};
      CHECK( ma.extent(0) == 3 );
      CHECK( v[0] == 0. );
      CHECK( v[1] == 1. );
      CHECK( v[2] == 2. );
    }

    SUBCASE("2D") {
      std::vector<double> v(6);
      dyn_multi_array_view<double,2> ma = {{{0,1},{2,3},{4,5}},v.data()};
      CHECK( ma.extent(0) == 3 );
      CHECK( ma.extent(1) == 2 );
      // remember that dyn_multi_array is Fortran-ordered
      CHECK( v[0] == 0. );
      CHECK( v[1] == 2. );
      CHECK( v[2] == 4. );
      CHECK( v[3] == 1. );
      CHECK( v[4] == 3. );
      CHECK( v[5] == 5. );
    }
  }
}
// [Sphinx Doc] multi_array constructors - init lists }

// [Sphinx Doc] multi_array constructors - low-level {
TEST_CASE("multi_array constructors - low-level") {

  SUBCASE("from range and shape") {
    dyn_shape<int,2> sh = {3,2}; // no offset
    std::vector<double> v = {0.,1.,2.,3.,4.,5.};
    dyn_multi_array<double,2> ma(std::move(v),std::move(sh));

    CHECK( ma(0,0) == 0. ); CHECK( ma(0,1) == 3. );
    CHECK( ma(1,0) == 1. ); CHECK( ma(1,1) == 4. );
    CHECK( ma(2,0) == 2. ); CHECK( ma(2,1) == 5. );
  }


  SUBCASE("from range and extent") {
    std::vector<double> v = {0.,1.,2.,3.,4.,5.};
    dyn_multi_array<double,2> ma(std::move(v),3,2);

    CHECK( ma(0,0) == 0. ); CHECK( ma(0,1) == 3. );
    CHECK( ma(1,0) == 1. ); CHECK( ma(1,1) == 4. );
    CHECK( ma(2,0) == 2. ); CHECK( ma(2,1) == 5. );
  }


  SUBCASE("from range only") {
    std::vector<double> v = {0.,1.,2.,3.,4.,5.};
    dyn_multi_array<double,2> ma(std::move(v));
    CHECK( ma.extent() == zero_multi_index<int,2> );

    SUBCASE("reshape with same size as underlying range") {
      reshape(ma,{3,2});
      CHECK( ma(0,0) == 0. ); CHECK( ma(0,1) == 3. );
      CHECK( ma(1,0) == 1. ); CHECK( ma(1,1) == 4. );
      CHECK( ma(2,0) == 2. ); CHECK( ma(2,1) == 5. );
    }

    SUBCASE("reshape with bigger size") {
      reshape(ma,{4,2});
      ma(2,1) = 6; // previously undefined
      ma(3,1) = 7; // previously undefined
      CHECK( ma(0,0) == 0. ); CHECK( ma(0,1) == 4. );
      CHECK( ma(1,0) == 1. ); CHECK( ma(1,1) == 5. );
      CHECK( ma(2,0) == 2. ); CHECK( ma(2,1) == 6. );
      CHECK( ma(3,0) == 3. ); CHECK( ma(3,1) == 7. );
    }

    SUBCASE("reshape with smaller size") {
      reshape(ma,{2,2});
      CHECK( ma(0,0) == 0. ); CHECK( ma(0,1) == 2. );
      CHECK( ma(1,0) == 1. ); CHECK( ma(1,1) == 3. );
    }
  }
}
// [Sphinx Doc] multi_array constructors - low-level }
