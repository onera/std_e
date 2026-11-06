#include "std_e/unit_test/doctest.hpp"

#include "std_e/field/field.hpp"

using namespace std_e;

TEST_CASE("field equality") {
  scalar_field x = {0.,1.};
  scalar_field y = {0.,1.};
  scalar_field z = {0.,1.,2.};
  scalar_field t = {0.,3.};

  CHECK( x == y );
  CHECK( y == x );
  CHECK( x != z );
  CHECK( x != t );
}

TEST_CASE("field API") {
  scalar_field x = {0.,1.,2.,3.,4.};

  CHECK( x.n_element() == 5 );
  CHECK( x(0) == 0. );
  CHECK( x(1) == 1. );
  CHECK( x(2) == 2. );
  CHECK( x(3) == 3. );
  CHECK( x(4) == 4. );

  auto sub_x = x.sub(3);
  CHECK( sub_x.n_element() == 3 );
  CHECK( sub_x(0) == 0. );
  CHECK( sub_x(1) == 1. );
  CHECK( sub_x(2) == 2. );
}

TEST_CASE("owning field is regular") {
  scalar_field x = {0.,1.};
  scalar_field y = {3.,4.,5.};

  x = y;
  x(0) = 10.;

  CHECK( x.n_element() == 3 );
  CHECK( y.n_element() == 3 );
  CHECK( x(0) == 10. );
  CHECK( x(1) == 4. );
  CHECK( x(2) == 5. );
  CHECK( y(0) == 3. );
  CHECK( y(1) == 4. );
  CHECK( y(2) == 5. );
}

TEST_CASE("field_view") {
  scalar_field x = {0.,1.,2.,3.,4.};

  scalar_field_view v(x);
  CHECK( v.n_element() == 5 );

  CHECK( v(0) == 0. );
  CHECK( v(1) == 1. );
  CHECK( v(2) == 2. );
  CHECK( v(3) == 3. );
  CHECK( v(4) == 4. );

  v(0) = 5.;
  v(1) = 6.;
  v(2) = 7.;
  v(3) = 8.;
  v(4) = 9.;

  CHECK( x(0) == 5. );
  CHECK( x(1) == 6. );
  CHECK( x(2) == 7. );
  CHECK( x(3) == 8. );
  CHECK( x(4) == 9. );
}

TEST_CASE("field_const_view") {
  const scalar_field cx = {0.,1.,2.,3.,4.};

  scalar_field_const_view cvx(cx);
  CHECK( cvx.n_element() == 5 );

  CHECK( cvx(0) == 0. );
  CHECK( cvx(1) == 1. );
  CHECK( cvx(2) == 2. );
  CHECK( cvx(3) == 3. );
  CHECK( cvx(4) == 4. );

  scalar_field y = {5.,6.,7.,8.,9.};

  scalar_field_const_view cvy(y);
  CHECK( cvy.n_element() == 5 );

  CHECK( cvy(0) == 5. );
  CHECK( cvy(1) == 6. );
  CHECK( cvy(2) == 7. );
  CHECK( cvy(3) == 8. );
  CHECK( cvy(4) == 9. );
}

TEST_CASE("field_view VS. field_ref") {
  scalar_field x = {0.,1.,2.,3.,4.};
  scalar_field y = {5.,6.,7.,8.,9.};

  SUBCASE("a view is attached to an object") {
    // Create the view to point to x
    scalar_field_view v(x);
    CHECK( v(0) == 0. );
    CHECK( v(1) == 1. );
    CHECK( v(2) == 2. );
    CHECK( v(3) == 3. );
    CHECK( v(4) == 4. );

    // Now make view point to y
    v = y;
    CHECK( v(0) == 5. );
    CHECK( v(1) == 6. );
    CHECK( v(2) == 7. );
    CHECK( v(3) == 8. );
    CHECK( v(4) == 9. );

    // x and y did not change
    CHECK( x(0) == 0. );
    CHECK( x(1) == 1. );
    CHECK( x(2) == 2. );
    CHECK( x(3) == 3. );
    CHECK( x(4) == 4. );

    CHECK( y(0) == 5. );
    CHECK( y(1) == 6. );
    CHECK( y(2) == 7. );
    CHECK( y(3) == 8. );
    CHECK( y(4) == 9. );
  }

  SUBCASE("a reference assigns through") {
    // Create the ref to point to x. It will always point to x
    scalar_field_ref rx(x);
    CHECK( rx(0) == 0. );
    CHECK( rx(1) == 1. );
    CHECK( rx(2) == 2. );
    CHECK( rx(3) == 3. );
    CHECK( rx(4) == 4. );

    // Assign y to rx, and hence to x
    rx = y;
    CHECK( rx(0) == 5. );
    CHECK( rx(1) == 6. );
    CHECK( rx(2) == 7. );
    CHECK( rx(3) == 8. );
    CHECK( rx(4) == 9. );

    // x has been changed through rx
    CHECK( x(0) == 5. );
    CHECK( x(1) == 6. );
    CHECK( x(2) == 7. );
    CHECK( x(3) == 8. );
    CHECK( x(4) == 9. );

    CHECK( y(0) == 5. );
    CHECK( y(1) == 6. );
    CHECK( y(2) == 7. );
    CHECK( y(3) == 8. );
    CHECK( y(4) == 9. );
  }
}
