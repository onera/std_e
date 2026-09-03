#include "std_e/unit_test/doctest.hpp"

#include "std_e/field/md_field_uniform.hpp"

using namespace std_e;


TEST_CASE("md_field_uniform") {
  SUBCASE("vector") {
    vector_field_uniform<3> x = {4.,5.,6.};

    CHECK( x(0) == 4. );
    CHECK( x(1) == 5. );
    CHECK( x(2) == 6. );
  }
  SUBCASE("tensor") {
    tensor_field_uniform<3,2> x = {
      {1.,2.},
      {3.,4.},
      {5.,6.}
    };
    CHECK( x(0,0) ==  1.); CHECK( x(0,1) ==  2.);
    CHECK( x(1,0) ==  3.); CHECK( x(1,1) ==  4.);
    CHECK( x(2,0) ==  5.); CHECK( x(2,1) ==  6.);
  }
  SUBCASE("equality") {
    vector_field_uniform<2> x = {0., 1. };
    vector_field_uniform<2> y = {0., 1. };
    vector_field_uniform<2> z = {0., 1.1};

    CHECK( x == y );
    CHECK( x != z );
    CHECK( x < z );
  }
}
