#include "std_e/unit_test/doctest.hpp"
#include "std_e/meta/type_traits.hpp"

using namespace std_e;


TEST_CASE("are_integrals") {
  CHECK( are_integrals<> );

  CHECK( are_integrals<int> );
  CHECK( are_integrals<int,long> );
  CHECK( are_integrals<int,int> );

  CHECK_FALSE( are_integrals<double> );
  CHECK_FALSE( are_integrals<int,double> );
  CHECK_FALSE( are_integrals<int,double,int> );
}
