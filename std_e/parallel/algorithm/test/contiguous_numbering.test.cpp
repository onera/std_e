#include "std_e/unit_test/doctest.hpp"
#include "std_e/parallel/algorithm/contiguous_numbering.hpp"
#include <vector>

using std_e::binary_search;

TEST_CASE("binary_search") {
  std::vector<int> v = {4,5,5,8,10};

  CHECK( binary_search(v,  0) == -1 );
  CHECK( binary_search(v,  4) ==  0 );
  CHECK( binary_search(v,  5) ==  2 );
  CHECK( binary_search(v,  6) ==  2 );
  CHECK( binary_search(v,  8) ==  3 );
  CHECK( binary_search(v, 10) ==  4 );
  CHECK( binary_search(v, 11) == -1 );
}
