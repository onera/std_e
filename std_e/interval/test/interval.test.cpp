#include "std_e/unit_test/doctest.hpp"

#include "std_e/interval/interval.hpp"
#include <vector>


using namespace std;
using namespace std_e;


// [Sphinx Doc] interval {
TEST_CASE("interval basic use") {
  std_e::interval r = {40,50};

  SUBCASE("length") {
    CHECK( length(r) == 10 );
  }

  SUBCASE("first last") {
    CHECK( r.first() == 40 );
    CHECK( r.last () == 50 );
  }

  SUBCASE("is_in") {
    CHECK( !is_in(r,39) );

    CHECK(  is_in(r,40) );
    CHECK(  is_in(r,43) );
    // ...
    CHECK(  is_in(r,49) );

    CHECK( !is_in(r,50) );
  }
}
// [Sphinx Doc] interval }

TEST_CASE("interval -- more checks") {
  SUBCASE("equality") {
    std_e::interval r = {40,50};
    std_e::interval s = {40,50};
    std_e::interval t = {41,51};
    std_e::interval u = {41,50};
    CHECK( s == r );
    CHECK( t != r );
    CHECK( u != r );
  }

  SUBCASE("assignment") {
    std_e::interval t = {100,200};
    const std_e::interval u = {0,1};
    t = u;
    CHECK( t.first() == 0 );
    CHECK( t.last() == 1 );
  }

  SUBCASE("contiguous") {
    std_e::interval r = {40,50};
    std_e::interval s = {50,60};
    std_e::interval t = {49,60};
    std_e::interval u = {51,60};

    CHECK(  contiguous(r,s) );
    CHECK( !contiguous(s,r) );
    CHECK( !contiguous(r,t) );
    CHECK( !contiguous(r,u) );
  }
}

// [Sphinx Doc] closed_interval {
TEST_CASE("closed_interval") {
  std_e::closed_interval r = {40,50};

  SUBCASE("length") {
    CHECK( length(r) == 11 );
  }

  SUBCASE("first last") {
    CHECK( r.first() == 40 );
    CHECK( r.last () == 50 );
  }

  SUBCASE("is_in") {
    CHECK( !is_in(r,39) );

    CHECK(  is_in(r,40) );
    CHECK(  is_in(r,43) );
    // ...
    CHECK(  is_in(r,49) );
    CHECK(  is_in(r,50) );

    CHECK( !is_in(r,51) );
  }

  SUBCASE("to_closed_open") {
    CHECK( to_closed_open(r) == interval{40,51} );
  }
}
// [Sphinx Doc] closed_interval }

TEST_CASE("interval -- more checks") {
  SUBCASE("contiguous") {
    std_e::closed_interval r = {40,50};
    std_e::closed_interval s = {51,60};
    std_e::closed_interval t = {50,60};
    std_e::closed_interval u = {52,60};

    CHECK(  contiguous(r,s) );
    CHECK( !contiguous(s,r) );
    CHECK( !contiguous(r,t) );
    CHECK( !contiguous(r,u) );
  }
}
