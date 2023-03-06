#include "std_e/unit_test/doctest.hpp"
#include "std_e/algorithm/distribution/uniform_sample.hpp"


using namespace std_e;
using std::vector;


TEST_CASE("uniform_sample") {
  vector<int> x = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14};

  CHECK( uniform_sample(x,0) == vector<int>{} );
  CHECK( uniform_sample(x,1) == vector{7} );
  CHECK( uniform_sample(x,2) == vector{0,14} );
  CHECK( uniform_sample(x,3) == vector{0,7,14} );

  CHECK( uniform_sample(x,10) == vector{0,2,4,6,8,10,11,12,13,14} );
  // Note: This is not very good since its biased towards the end (more samples at the end).
  //       It comes from the fact that non-exact division is rounded up at the begining and rounded down at the end.
  //       A better alternative would be to spread up-rounding accross the sub-intervals.

  CHECK( uniform_sample(x,15) == x );

  SUBCASE("smaller") {
    vector<int> x = {0,1,2,3,4};
    CHECK( uniform_sample(x,2) == vector{0,4} );
  }
}

TEST_CASE("uniform_sample_exclude_ends") {
  vector<int> x = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14};

  CHECK( uniform_sample_exclude_ends(x,0) == vector<int>{} );
  CHECK( uniform_sample_exclude_ends(x,1) == vector{7} );
  CHECK( uniform_sample_exclude_ends(x,2) == vector{5,10} );
  CHECK( uniform_sample_exclude_ends(x,3) == vector{4,8,11} );

  CHECK( uniform_sample_exclude_ends(x,10) == vector{2,4,6,7,8,9,10,11,12,13} );
  // Note: same bias as `uniform_sample`

  // force to give the edges anyway since not enought samples
  CHECK( uniform_sample_exclude_ends(x,14) == vector{1,2,3,4,5,6,7,8,9,10,11,12,13,14} ); // bias towards the end to the coherent with previous
  CHECK( uniform_sample_exclude_ends(x,15) == x );
}
