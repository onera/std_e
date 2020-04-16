#include "std_e/unit_test/doctest.hpp"

#include "std_e/interval/interval.hpp"
#include <vector>


using namespace std;
using namespace std_e;


TEST_CASE("interval") {
  std_e::interval<int> r = {42,52};

  CHECK(is_in(42,r));
  CHECK(is_in(43,r));
  // ...
  CHECK(is_in(51,r));

  CHECK_FALSE(is_in(52,r));

  CHECK( size(r) == 10 );
}


TEST_CASE("is_interval") {
  vector<int> inter = {2,3,4,5};    
  CHECK(is_interval(inter));

  vector<int> non_inter0 = {2,3,  5};    
  CHECK_FALSE(is_interval(non_inter0));

  vector<int> non_inter1 = {2,3,4,5,4};    
  CHECK_FALSE(is_interval(non_inter1));
}
