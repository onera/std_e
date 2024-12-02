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
    std_e::dynarray<double> off = {1., 2.7, 3.14, 10.};

    CHECK( interval_index( 1.  ,off) == 0 );
    CHECK( interval_index( 2.7 ,off) == 1 );
    CHECK( interval_index( 3.14,off) == 2 );
    CHECK( interval_index( 10. ,off) == 3 ); // semi-open interval: 10. does not belong to the last interval

    CHECK( interval_index( 0.5 ,off) ==-1 ); // 0.5 should be inserted before the first interval (i.e. interval 0)
    CHECK( interval_index( 2.  ,off) == 0 );
    CHECK( interval_index( 3.  ,off) == 1 );
    CHECK( interval_index( 5.  ,off) == 2 );
    CHECK( interval_index(15.  ,off) == 3 ); // 15 should be inserted after the last interval (i.e. interval 2)
  }

  SUBCASE("zero-length interval") {
    std_e::dynarray<int> off = {10};

    CHECK( interval_index(  9 ,off) == -1 ); // 9 should be inserted before
    CHECK( interval_index( 11 ,off) ==  0 ); // 11 should be inserted after (here, since n_interval()==0, index 0 is indeed after!)
    CHECK( interval_index( 10 ,off) ==  0 ); // same as 11: because of semi-openness, 10 is already past the interval
  }

  SUBCASE("zero-length sub-intervals") {
    std_e::dynarray<int> off = {0,10,10,10,10,20};

    CHECK( interval_index( -10 ,off) == -1 );
    CHECK( interval_index(   0 ,off) ==  0 );
    CHECK( interval_index(   5 ,off) ==  0 );
    CHECK( interval_index(   9 ,off) ==  0 );
    CHECK( interval_index(  10 ,off) ==  4 ); // the-interval [10,20) is at position 4. Previous intervals are [10,10), so they are empty
    CHECK( interval_index(  11 ,off) ==  4 );
    CHECK( interval_index(  20 ,off) ==  5 );
    CHECK( interval_index(  21 ,off) ==  5 );
  }
}


TEST_CASE("offset_to_stride") {
  SUBCASE("base") {
    std_e::dynarray<int> off = {3,10,21,50};

    std_e::dynarray<int> bsz = std_e::offset_to_stride(off);

    REQUIRE( bsz.size() == 3 );
    CHECK( bsz[0] == 7 );
    CHECK( bsz[1] == 11 );
    CHECK( bsz[2] == 29 );
  }

  SUBCASE("zero-length") {
    std_e::dynarray<int> off = {10};

    std_e::dynarray<int> bsz = std_e::offset_to_stride(off);

    REQUIRE( bsz.size() == 0 );
  }
}

TEST_CASE("stride_to_offset") {
  SUBCASE("base") {
    std_e::dynarray<int> bsz = {7,11,29};

    std_e::dynarray<int> off = std_e::stride_to_offset(bsz);

    REQUIRE( off.size() == 4 );
    CHECK( off[0] == 0 );
    CHECK( off[1] == 7 );
    CHECK( off[2] == 7+11 );
    CHECK( off[3] == 7+11+29 );
  }

  SUBCASE("no element") {
    std_e::dynarray<int> bsz = {};

    std_e::dynarray<int> off = std_e::stride_to_offset(bsz);

    REQUIRE( off.size() == 1 );
    CHECK( off[0] == 0 );
  }
}
TEST_CASE("inplace_stride_to_offset") {
  std_e::dynarray<int> bsz = {7,11,29, -1};
  auto& off = std_e::inplace_stride_to_offset(bsz);
  
  CHECK( &off == &bsz );
  CHECK( off == std_e::dynarray<int>{0,  7,  7+11,  7+11+29} );
}
