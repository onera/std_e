#include "std_e/unit_test/doctest.hpp"

#include "std_e/iterator/iterator_range.hpp"
#include <vector>

using std::vector;
using namespace std_e;

TEST_CASE("iterator_range") {
  std::vector<int> v = {9,2,5};

  iterator_range itr = {begin(v),end(v)};

  REQUIRE(itr.size() == 3);
  CHECK(itr[0] == 9 );
  CHECK(itr[1] == 2 );
  CHECK(itr[2] == 5 );
}
