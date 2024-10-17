#include "std_e/unit_test/doctest.hpp"
#include "std_e/algorithm/interval.hpp"


TEST_CASE("is_interval") {
  CHECK_FALSE( is_interval(std_e::dynarray<int>{}) );
  CHECK      ( is_interval(std_e::dynarray<int>{0}) );
  CHECK      ( is_interval(std_e::dynarray<int>{1}) );
  CHECK      ( is_interval(std_e::dynarray<int>{0,0}) );
  CHECK      ( is_interval(std_e::dynarray<int>{0,1}) );
  CHECK_FALSE( is_interval(std_e::dynarray<int>{1,0}) );
}

TEST_CASE("interval_index") {
  SUBCASE("base") {
    std_e::dynarray<double> idx = {1., 2.7, 3.14, 10.};

    CHECK( interval_index( 1.  ,idx) == 0 );
    CHECK( interval_index( 2.7 ,idx) == 1 );
    CHECK( interval_index( 3.14,idx) == 2 );
    CHECK( interval_index( 10. ,idx) == 3 ); // semi-open interval: 10. does not belong to the last interval

    CHECK( interval_index( 0.5 ,idx) ==-1 ); // 0.5 should be inserted before the first interval (i.e. interval 0)
    CHECK( interval_index( 2.  ,idx) == 0 );
    CHECK( interval_index( 3.  ,idx) == 1 );
    CHECK( interval_index( 5.  ,idx) == 2 );
    CHECK( interval_index(15.  ,idx) == 3 ); // 15 should be inserted after the last interval (i.e. interval 2)
  }

  SUBCASE("zero-length interval") {
    std_e::dynarray<int> idx = {10};

    CHECK( interval_index(  9 ,idx) == -1 ); // 9 should be inserted before
    CHECK( interval_index( 11 ,idx) ==  0 ); // 11 should be inserted after (here, since n_interval()==0, index 0 is indeed after!)
    CHECK( interval_index( 10 ,idx) ==  0 ); // same as 11: because of semi-openness, 10 is already past the interval
  }

  SUBCASE("zero-length sub-intervals") {
    std_e::dynarray<int> idx = {0,10,10,10,10,20};

    CHECK( interval_index( -10 ,idx) == -1 );
    CHECK( interval_index(   0 ,idx) ==  0 );
    CHECK( interval_index(   5 ,idx) ==  0 );
    CHECK( interval_index(   9 ,idx) ==  0 );
    CHECK( interval_index(  10 ,idx) ==  4 ); // the-interval [10,20) is at position 4. Previous intervals are [10,10), so they are empty
    CHECK( interval_index(  11 ,idx) ==  4 );
    CHECK( interval_index(  20 ,idx) ==  5 );
    CHECK( interval_index(  21 ,idx) ==  5 );
  }
}


TEST_CASE("index_to_size") {
  SUBCASE("base") {
    std_e::dynarray<int> idx = {3,10,21,50};

    std_e::dynarray<int> bsz = std_e::index_to_size(idx);

    REQUIRE( bsz.size() == 3 );
    CHECK( bsz[0] == 7 );
    CHECK( bsz[1] == 11 );
    CHECK( bsz[2] == 29 );
  }

  SUBCASE("zero-length") {
    std_e::dynarray<int> idx = {10};

    std_e::dynarray<int> bsz = std_e::index_to_size(idx);

    REQUIRE( bsz.size() == 0 );
  }
}

TEST_CASE("size_to_index") {
  SUBCASE("base") {
    std_e::dynarray<int> bsz = {7,11,29};

    std_e::dynarray<int> idx = std_e::size_to_index(bsz);

    REQUIRE( idx.size() == 4 );
    CHECK( idx[0] == 0 );
    CHECK( idx[1] == 7 );
    CHECK( idx[2] == 7+11 );
    CHECK( idx[3] == 7+11+29 );
  }

  SUBCASE("no element") {
    std_e::dynarray<int> bsz = {};

    std_e::dynarray<int> idx = std_e::size_to_index(bsz);

    REQUIRE( idx.size() == 1 );
    CHECK( idx[0] == 0 );
  }
}
TEST_CASE("inplace_size_to_index") {
  std_e::dynarray<int> bsz = {7,11,29, -1};
  auto& idx = std_e::inplace_size_to_index(bsz);
  
  CHECK( &idx == &bsz );
  CHECK( idx == std_e::dynarray<int>{0,  7,  7+11,  7+11+29} );
}
