#include "std_e/unit_test/doctest.hpp"
#include "std_e/meta/type_traits.hpp"

using namespace std_e;


TEST_CASE("are_integral") {
  CHECK( are_integral<> );

  CHECK( are_integral<int> );
  CHECK( are_integral<int,long> );
  CHECK( are_integral<int,int> );

  CHECK_FALSE( are_integral<double> );
  CHECK_FALSE( are_integral<int,double> );
  CHECK_FALSE( are_integral<int,double,int> );
}
