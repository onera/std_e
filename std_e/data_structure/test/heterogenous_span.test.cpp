#include "std_e/unit_test/doctest.hpp"
#include "std_e/data_structure/heterogenous_span.hpp"

using namespace std;
using namespace std_e;

TEST_CASE("hspan") {
  vector<int> vi = {1,2,3,4};
  vector<double> vd = {3.14,2.7};
  hspan<int,double> hv = { make_span(vi) , make_span(vd) };

  SUBCASE("hsize") {
    CHECK( hspan<int,double>::hsize() == 2 );
    CHECK( hv.hsize() == 2 );
  }
  SUBCASE("size") {
    CHECK( hv.size() == 4+2 );
  }

  SUBCASE("assessors") {
    CHECK( std_e::get<int>(hv) == vector{1,2,3,4} );
    CHECK( std_e::get<double>(hv) == vector{3.14,2.7} );
  }
  SUBCASE("assessors from index") {
    CHECK( std_e::get<0>(hv) == vector{1,2,3,4} );
    CHECK( std_e::get<1>(hv) == vector{3.14,2.7} );
  }

  SUBCASE("mutable accessors") {
    std_e::get<int>(hv)[0] = 100;

    CHECK( std_e::get<int>(hv) == vector{100,2,3,4} );

    // underlying data modified
    CHECK( vi                  == vector{100,2,3,4} );
  }
}
