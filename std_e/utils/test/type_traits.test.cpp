#include "doctest/doctest.h"
#include "std_e/utils/type_traits.h"

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
