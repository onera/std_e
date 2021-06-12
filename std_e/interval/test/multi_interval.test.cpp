#include "std_e/unit_test/doctest.hpp"

#include "std_e/interval/multi_interval.hpp"
#include <vector>


using namespace std;
using namespace std_e;


// [Sphinx Doc] multi_interval {
TEST_CASE("multi_interval basic use") {
  std_e::multi_interval<int> mr = {{1,10,100},{4,40,400}};

  SUBCASE("length") {
    CHECK( length(mr) == multi_index<int>{3,30,300} );
  }

  SUBCASE("first last") {
    CHECK( mr.first() == multi_index<int>{1,10,100} );
    CHECK( mr.last () == multi_index<int>{4,40,400} );
  }

  SUBCASE("operator[]") {
    CHECK( mr[0] == interval{1,4} );

    mr[1] = interval{15,45};
    CHECK( mr.first() == multi_index<int>{1,15,100} );
    CHECK( mr.last () == multi_index<int>{4,45,400} );
  }
}
// [Sphinx Doc] multi_interval }
