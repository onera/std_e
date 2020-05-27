#include "std_e/unit_test/doctest.hpp"
#include "std_e/multi_index/multi_index.hpp"

using namespace std;
using namespace std_e;

TEST_CASE("multi_index structured bindings") {
  multi_index<int,4> is = {5,4,3,2};
  auto [i,j,k,l] = is;
  CHECK ( i == 5 );
  CHECK ( j == 4 );
  CHECK ( k == 3 );
  CHECK ( l == 2 );
}
