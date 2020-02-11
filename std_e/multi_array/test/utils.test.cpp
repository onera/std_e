#include "doctest/doctest.h"
#include "std_e/multi_array/utils.hpp"
#include "std_e/multi_array/multi_array.hpp"

using namespace std_e;

TEST_CASE("multi_array to_string") {
  // FIXME
  //fixed_multi_array<int> f0; // note: no dimensions means rank 0 (i.e. empty)
  //CHECK( to_string(f0) == "[]" );

  fixed_multi_array<int,4> f1 = {7,8,9,10}; // one-dimensional
  CHECK( to_string(f1) == "[7,8,9,10]" );

  fixed_multi_array<int,2,3> f2 = {{1,2,3},{4,5,6}};
  CHECK( to_string(f2) == "[1,2,3;4,5,6]" );

// degenerate cases
  fixed_multi_array<int,1,3> f3 = {{1,2,3}};
  CHECK( to_string(f3) == "[1,2,3]" );

  // DOES NOT WORK: ambiguous
  //fixed_multi_array<int,1,3> f4 = {{1},{2},{3}};
  //CHECK( to_string(f4) == "[1;2;3]" );

  // DOES NOT WORK: ambiguous
  //fixed_multi_array<int,1> f5 = {42};
  //CHECK( to_string(f5) == "[42]" );
}
