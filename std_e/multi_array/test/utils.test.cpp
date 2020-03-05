#include "doctest/doctest.h"
#include "std_e/multi_array/utils.hpp"
#include "std_e/multi_array/multi_array.hpp"

using namespace std_e;


TEST_CASE("multi_array is_empty") {
  SUBCASE("multi_arrays of rank 0 are scalars") {
    SUBCASE("fixed arrays") {
      fixed_multi_array<int> f; // note: no dimensions means rank 0, i.e. scalar
      CHECK( f.size() == 1 );
      CHECK( !is_empty(f) );
    }
    SUBCASE("dyn arrays") {
      // NOTE: not well supported
      dyn_multi_array<int,int32_t,0> f;
      CHECK( f.size() == 1 );
      // WARNING: nothing has been allocated!
      // The following would segfault
      // f_no_alloc() = 42; // BAD: SEGFAULT

      CHECK( !is_empty(f) );
    }
  }

  SUBCASE("fixed arrays") {
    fixed_multi_array<int,0,0> f_0_0;
    CHECK( is_empty(f_0_0) );

    fixed_multi_array<int,4,3> f_4_3;
    CHECK( !is_empty(f_4_3) );
  }
  SUBCASE("dyn arrays") {
    dyn_multi_array<int,int32_t,2> f_0_0(0,0);
    CHECK( is_empty(f_0_0) );

    dyn_multi_array<int,int32_t,2> f_4_3(4,3);
    CHECK( !is_empty(f_4_3) );
  }
}

TEST_CASE("multi_array to_string") {
  fixed_multi_array<int> f0; // note: no dimensions means rank 0, i.e. scalar
  f0() = 42;
  CHECK( to_string(f0) == "[42]" );

  fixed_multi_array<int,4> f1 = {7,8,9,10}; // one-dimensional
  CHECK( to_string(f1) == "[7,8,9,10]" );

  fixed_multi_array<int,2,3> f2 = {{1,2,3},{4,5,6}};
  CHECK( to_string(f2) == "[1,2,3;4,5,6]" );

// degenerate cases
  fixed_multi_array<int,1,3> f3 = {{1,2,3}};
  CHECK( to_string(f3) == "[1,2,3]" );

  // TODO DOES NOT WORK: ambiguous
  //fixed_multi_array<int,3,1> f4 = {{1},{2},{3}};
  //CHECK( to_string(f4) == "[1;2;3]" );

  fixed_multi_array<int,1> f5 = {42};
  CHECK( to_string(f5) == "[42]" );
}
