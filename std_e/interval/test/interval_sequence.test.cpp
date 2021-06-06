#include "std_e/unit_test/doctest.hpp"


#include "std_e/interval/interval_sequence.hpp"


// [Sphinx Doc] interval_vector {
TEST_CASE("interval_vector") {
  std_e::interval_vector<int> iv = {5,10,100,120};

// operator[]
  CHECK( iv[0] == 5 );
  CHECK( iv[1] == 10 );
  CHECK( iv[2] == 100 );
  CHECK( iv[3] == 120 );

// global
  CHECK( iv.n_interval() == 3 );

  CHECK( iv.inf() == 5 );
  CHECK( iv.sup() == 120 );
  CHECK( iv.length() == 115 );
  CHECK( iv.interval() == std_e::interval{5,120} );

// interval by interval
  CHECK( iv.inf(0) == 5 );
  CHECK( iv.sup(0) == 10 );
  CHECK( iv.length(0) == 5 );
  CHECK( iv.interval(0) == std_e::interval{5,10} );

  CHECK( iv.inf(1) == 10 );
  CHECK( iv.sup(1) == 100 );
  CHECK( iv.length(1) == 90 );
  CHECK( iv.interval(1) == std_e::interval{10,100} );

  CHECK( iv.inf(2) == 100 );
  CHECK( iv.sup(2) == 120 );
  CHECK( iv.length(2) == 20 );
  CHECK( iv.interval(2) == std_e::interval{100,120} );
}
// [Sphinx Doc] interval_vector }

TEST_CASE("interval_vector from vector") {
  std::vector v = {3,10,21};
  std_e::interval_vector iv = std_e::to_interval_vector(std::move(v));

  CHECK( iv[0] == 3 );
  CHECK( iv[1] == 10 );
  CHECK( iv[2] == 21 );

  CHECK( iv.n_interval() == 2 );
}


TEST_CASE("interval_vector push_back") {
  std_e::interval_vector<int> iv;
  iv.push_back(3);
  iv.push_back(10);
  iv.push_back(21);

  CHECK( iv[0] == 3 );
  CHECK( iv[1] == 10 );
  CHECK( iv[2] == 21 );

  CHECK( iv.n_interval() == 2 );
}

TEST_CASE("interval_lengths") {
  std_e::interval_vector<int> iv = {3,10,21,50};

  std::vector<int> il = std_e::interval_lengths(iv);

  CHECK( il[0] == 7 );
  CHECK( il[1] == 11 );
  CHECK( il[2] == 29 );
}

TEST_CASE("interval_index") {
  std_e::interval_vector<double> iv = {1., 2.7, 3.14, 10.};

  CHECK( interval_index( 1.  ,iv) == 0 );
  CHECK( interval_index( 2.7 ,iv) == 1 );
  CHECK( interval_index( 3.14,iv) == 2 );
  CHECK( interval_index( 10. ,iv) == 3 ); // semi-open interval: 10. does not belong to the last interval

  CHECK( interval_index( 0.5 ,iv) ==-1 ); // 0.5 should be inserted before the first interval (i.e. interval 0)
  CHECK( interval_index( 2.  ,iv) == 0 );
  CHECK( interval_index( 3.  ,iv) == 1 );
  CHECK( interval_index( 5.  ,iv) == 2 );
  CHECK( interval_index(15.  ,iv) == 3 ); // 15 should be inserted after the last interval (i.e. interval 2)
}
