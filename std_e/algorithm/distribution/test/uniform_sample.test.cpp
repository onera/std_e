#include "std_e/unit_test/doctest.hpp"
#include "std_e/algorithm/distribution/uniform_sample.hpp"


using namespace std_e;


TEST_CASE("uniform_sample") {
  std::vector<int> x = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14};

  CHECK( uniform_sample(x,0) == std::vector<int>{} );
  CHECK( uniform_sample(x,1) == std::vector{7} );
  CHECK( uniform_sample(x,2) == std::vector{3,11} );
  CHECK( uniform_sample(x,3) == std::vector{2,7,12} );

  CHECK( uniform_sample(x,10) == std::vector{0,2,4,6,8,10,11,12,13,14} );
  // Note: This is not very good since its biased towards the end (more samples at the end).
  //       It comes from the fact that non-exact division is rounded up at the begining and rounded down at the end.
  //       A better alternative would be to spread up-rounding accross the sub-intervals.

  CHECK( uniform_sample(x,15) == x );
}
