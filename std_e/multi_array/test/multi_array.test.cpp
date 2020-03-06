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
  CHECK( ma.extent(0) == 3 );
  CHECK( ma.extent(1) == 2 );

  CHECK( ma(0,0) == 1 );
  CHECK( ma(1,0) == 2 );
  CHECK( ma(2,0) == 3 );
  CHECK( ma(0,1) == 4 );
  CHECK( ma(1,1) == 5 );
  CHECK( ma(2,1) == 6 );
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
