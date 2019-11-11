#include "doctest/doctest.h"
#include "std_e/algorithm/algorithm.hpp"
#include <vector>

using namespace std_e;

TEST_CASE("equal_n") {
  std::vector<int> v0 = {2,3,4,5};

  std::vector<int> v1 = {2,3,4,5};
  CHECK( equal_n(begin(v0),begin(v1),4) );

  std::vector<int> v2 = {42,3,4,5};
  CHECK_FALSE( equal_n(begin(v0),begin(v2),4) );

  std::vector<int> v3 = {2,3,4,42};
  CHECK_FALSE( equal_n(begin(v0),begin(v3),4) );

  std::vector<int> v4 = {2,3,4};
  CHECK( equal_n(begin(v0),begin(v4),3) );
}

TEST_CASE("equal_n with predicate") {
  auto pred = [](int i, int j){ return i%2 == j%5; };

  std::vector<int> v0 = {2,3,4,5};

  std::vector<int> v1 = {0,1,0,1};
  CHECK( equal_n(begin(v0),begin(v1),4,pred) );

  std::vector<int> v2 = {1,1,0,1};
  CHECK_FALSE( equal_n(begin(v0),begin(v2),4,pred) );

  std::vector<int> v3 = {0,1,0};
  CHECK( equal_n(begin(v0),begin(v3),3,pred) );
}
