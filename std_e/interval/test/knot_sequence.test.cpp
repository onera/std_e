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


TEST_CASE("int_knot_vector from initialization list") {
  std_e::int_knot_vector kv = {3,10,21};

  CHECK( kv[0] == 3 );
  CHECK( kv[1] == 10 );
  CHECK( kv[2] == 21 );

  CHECK( kv.nb_intervals() == 2 );
}

TEST_CASE("interval_lengths") {
  std_e::int_knot_vector kv = {3,10,21,50};

  std::vector<int> il = std_e::interval_lengths(kv);

  CHECK( il[0] == 7 );
  CHECK( il[1] == 11 );
  CHECK( il[2] == 29 );
}

TEST_CASE("interval_index") {
  std_e::knot_vector<double> kv = {1., 2.7, 3.14, 10.};

  CHECK( interval_index( 1.  ,kv) == 0 );
  CHECK( interval_index( 2.7 ,kv) == 1 );
  CHECK( interval_index( 3.14,kv) == 2 );
  CHECK( interval_index( 10,  kv) == 3 ); // semi-open interval: 10 does not belong to the last interval

  CHECK( interval_index( 0.5 ,kv) ==-1 ); // 0.5 should be inserted before the first interval (i.e. interval 0)
  CHECK( interval_index( 2.  ,kv) == 0 );
  CHECK( interval_index( 3.  ,kv) == 1 );
  CHECK( interval_index( 5.  ,kv) == 2 );
  CHECK( interval_index(15.  ,kv) == 3 ); // 15 should be inserted after the last interval (i.e. interval 2)
}
