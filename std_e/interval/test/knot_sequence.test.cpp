#include "std_e/unit_test/doctest.hpp"


#include "std_e/interval/knot_sequence.hpp"


TEST_CASE("int_knot_vector") {
  std_e::int_knot_vector kv;
  kv.push_back(1);
  kv.push_back(10);
  kv.push_back(100);
  kv.push_back(120);

// operator[]
  CHECK( kv[0] == 1 );
  CHECK( kv[1] == 10 );
  CHECK( kv[2] == 100 );
  CHECK( kv[3] == 120 );

// global
  CHECK( kv.nb_intervals() == 3 );

  CHECK( kv.inf() == 1 );
  CHECK( kv.sup() == 120 );
  CHECK( kv.length() == 119 );
  CHECK( kv.interval() == std_e::interval{1,120} );

// interval by interval
  CHECK( kv.inf(0) == 1 );
  CHECK( kv.sup(0) == 10 );
  CHECK( kv.length(0) == 9 );
  CHECK( kv.interval(0) == std_e::interval{1,10} );

  CHECK( kv.inf(1) == 10 );
  CHECK( kv.sup(1) == 100 );
  CHECK( kv.length(1) == 90 );
  CHECK( kv.interval(1) == std_e::interval{10,100} );

  CHECK( kv.inf(2) == 100 );
  CHECK( kv.sup(2) == 120 );
  CHECK( kv.length(2) == 20 );
  CHECK( kv.interval(2) == std_e::interval{100,120} );
}

TEST_CASE("int_knot_vector from vector") {
  std::vector<int> v = {3,10,21};
  std_e::int_knot_vector kv = std_e::to_knot_vector(std::move(v));

  CHECK( kv[0] == 3 );
  CHECK( kv[1] == 10 );
  CHECK( kv[2] == 21 );

  CHECK( kv.nb_intervals() == 2 );
}
