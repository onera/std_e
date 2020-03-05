#include "doctest/doctest.h"

#include "std_e/interval/interval_to_vector.hpp"

using namespace std_e;

TEST_CASE("interval to vector") {
  std::vector<int> inter = interval_to_vector(interval<int>{2,5});
  std::vector<int> inter_expected = {2,3,4};
  CHECK( inter == inter_expected );
}

TEST_CASE("zero interval to vector") {
  std::vector<int> inter = interval_to_vector(6);
  std::vector<int> inter_expected = {0,1,2,3,4,5};
  CHECK( inter == inter_expected );
}


