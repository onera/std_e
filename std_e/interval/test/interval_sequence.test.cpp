#if __cplusplus > 201703L
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

TEST_CASE("interval_vector deduction") {
  std_e::interval_vector iv = {5,10,100,120};
  CHECK( iv[0] == 5 );
  CHECK( iv[1] == 10 );
  CHECK( iv[2] == 100 );
  CHECK( iv[3] == 120 );
}

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
#endif // C++20
