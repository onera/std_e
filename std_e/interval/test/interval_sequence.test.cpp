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

TEST_CASE("interval_lengths") {
  SUBCASE("base") {
    std_e::interval_vector<int> iv = {3,10,21,50};

    std::vector<int> il = std_e::interval_lengths(iv);

    REQUIRE( il.size() == 3 );
    CHECK( il[0] == 7 );
    CHECK( il[1] == 11 );
    CHECK( il[2] == 29 );
  }

  SUBCASE("zero-length interval") {
    std_e::interval_vector<int> iv = {10};

    std::vector<int> il = std_e::interval_lengths(iv);

    REQUIRE( il.size() == 0 );
  }
}

TEST_CASE("indices_from_strides") {
  SUBCASE("base") {
    std::vector<int> v = {7,11,29};

    std_e::interval_vector<int> res = std_e::indices_from_strides(v);

    REQUIRE( res.size() == 4 );
    CHECK( res[0] == 0 );
    CHECK( res[1] == 7 );
    CHECK( res[2] == 7+11 );
    CHECK( res[3] == 7+11+29 );
  }

  SUBCASE("no element") {
    std::vector<int> v = {};

    std_e::interval_vector<int> res = std_e::indices_from_strides(v);

    REQUIRE( res.size() == 1 );
    CHECK( res[0] == 0 );
  }
}

TEST_CASE("interval_index") {
  SUBCASE("base") {
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

  SUBCASE("zero-length interval") {
    std_e::interval_vector<int> iv = {10};

    CHECK( interval_index(  9 ,iv) == -1 ); // 9 should be inserted before
    CHECK( interval_index( 11 ,iv) ==  0 ); // 11 should be inserted after (here, since n_interval()==0, index 0 is indeed after!)
    CHECK( interval_index( 10 ,iv) ==  0 ); // same as 11: because of semi-openness, 10 is already past the interval
  }

  SUBCASE("zero-length sub-intervals") {
    std_e::interval_vector<int> iv = {0,10,10,10,10,20};

    CHECK( interval_index( -10 ,iv) == -1 );
    CHECK( interval_index(   0 ,iv) ==  0 );
    CHECK( interval_index(   5 ,iv) ==  0 );
    CHECK( interval_index(   9 ,iv) ==  0 );
    CHECK( interval_index(  10 ,iv) ==  4 ); // the-interval [10,20) is at position 4. Previous intervals are [10,10), so they are empty
    CHECK( interval_index(  11 ,iv) ==  4 );
    CHECK( interval_index(  20 ,iv) ==  5 );
    CHECK( interval_index(  21 ,iv) ==  5 );
  }
}
#endif // C++20
