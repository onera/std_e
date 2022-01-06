#include "std_e/unit_test/doctest.hpp"
#include "std_e/algorithm/distribution/ticks_in_interval.hpp"
#include <vector>


using namespace std_e;
using std::vector;


TEST_CASE("number_of_ticks_in_interval") {
  SUBCASE("n>k") {
    int n = 4;
    int k = 2;
    /*  0   1   2   3
        |---|---|---|---|
             ^     ^
             |_____| the two ticks should end up here
    */
    CHECK( number_of_ticks_in_interval(n,k,0) == 0 );
    CHECK( number_of_ticks_in_interval(n,k,1) == 1 );
    CHECK( number_of_ticks_in_interval(n,k,2) == 1 );
    CHECK( number_of_ticks_in_interval(n,k,3) == 0 );
  }
  SUBCASE("n-1==k") {
    int n = 4;
    int k = 3;
    /*  0   1   2   3
        |---|---|---|---|
            ^   ^   ^
            |___|___|
    */
    CHECK( number_of_ticks_in_interval(n,k,0) == 0 );
    CHECK( number_of_ticks_in_interval(n,k,1) == 1 );
    CHECK( number_of_ticks_in_interval(n,k,2) == 1 );
    CHECK( number_of_ticks_in_interval(n,k,3) == 1 );
  }
  SUBCASE("n<k") {
    int n = 4;
    int k = 7;
    CHECK( number_of_ticks_in_interval(n,k,0) == 1 );
    CHECK( number_of_ticks_in_interval(n,k,1) == 2 );
    CHECK( number_of_ticks_in_interval(n,k,2) == 2 );
    CHECK( number_of_ticks_in_interval(n,k,3) == 2 );
  }
}

TEST_CASE("uniform_ticks_in_sub_interval") {

  SUBCASE("standard interval example: [0,22,54,60,86)") {

    SUBCASE("standard case example: position 5 samples") {
      //
      // 0-------------------22--------------------------------54------60-----------------------85
      //                      ^                       ^                      ^
      //                     22                      43                     64
      CHECK( uniform_ticks_in_sub_interval( 0,22,86,5) == vector{0} );
      CHECK( uniform_ticks_in_sub_interval(22,54,86,5) == vector{22,43} );
      CHECK( uniform_ticks_in_sub_interval(54,60,86,5) == vector<int>{} );
      CHECK( uniform_ticks_in_sub_interval(60,86,86,5) == vector{64,85} );

      // with an empty sub-interval
      CHECK( uniform_ticks_in_sub_interval(22,22,86,5) == vector<int>{} );

      // with the sub-interval being the complete interval
      CHECK( uniform_ticks_in_sub_interval(0,86,86,5) == vector{0,22,43,64,85} );
    }

    SUBCASE("small number of samples") {
      // 0-------------------22--------------------------------54------60-----------------------85
      // ^                                            ^                                          ^
      // 0                                           43 (middle)                                85

      // position 0 sample
      CHECK( uniform_ticks_in_sub_interval( 0,22,86,0) == vector<int>{} );
      CHECK( uniform_ticks_in_sub_interval(22,54,86,0) == vector<int>{} );
      CHECK( uniform_ticks_in_sub_interval(54,60,86,0) == vector<int>{} );
      CHECK( uniform_ticks_in_sub_interval(60,86,86,0) == vector<int>{} );

      // position 1 sample
      CHECK( uniform_ticks_in_sub_interval( 0,22,86,1) == vector<int>{} );
      CHECK( uniform_ticks_in_sub_interval(22,54,86,1) == vector{43} );
      CHECK( uniform_ticks_in_sub_interval(54,60,86,1) == vector<int>{} );
      CHECK( uniform_ticks_in_sub_interval(60,86,86,1) == vector<int>{} );

      // position 2 samples
      CHECK( uniform_ticks_in_sub_interval( 0,22,86,2) == vector{0} );
      CHECK( uniform_ticks_in_sub_interval(22,54,86,2) == vector<int>{} );
      CHECK( uniform_ticks_in_sub_interval(54,60,86,2) == vector<int>{} );
      CHECK( uniform_ticks_in_sub_interval(60,86,86,2) == vector{85} );

      // position 3 samples
      CHECK( uniform_ticks_in_sub_interval( 0,22,86,3) == vector{0} );
      CHECK( uniform_ticks_in_sub_interval(22,54,86,3) == vector{43} );
      CHECK( uniform_ticks_in_sub_interval(54,60,86,3) == vector<int>{} );
      CHECK( uniform_ticks_in_sub_interval(60,86,86,3) == vector{85} );
    }

    SUBCASE("lot of samples: effect of rounding") {
      // position 18 samples (exact division)
      CHECK( uniform_ticks_in_sub_interval( 0,22,86,18) == vector{0,5,10,15,20} );
      CHECK( uniform_ticks_in_sub_interval(22,54,86,18) == vector{25,30,35,40,45,50} );
      CHECK( uniform_ticks_in_sub_interval(54,60,86,18) == vector{55} );
      CHECK( uniform_ticks_in_sub_interval(60,86,86,18) == vector{60,65,70,75,80,85} );

      // position 12 samples
      //   Here we see the effect of rounding:
      //     at the beginning, we sample by steps of 8,
      //     then by steps of 7
      CHECK( uniform_ticks_in_sub_interval( 0,22,86,12) == vector{0,8,16} );
      CHECK( uniform_ticks_in_sub_interval(22,54,86,12) == vector{24,32,40,48} );
      CHECK( uniform_ticks_in_sub_interval(54,60,86,12) == vector{56} );
      CHECK( uniform_ticks_in_sub_interval(60,86,86,12) == vector{64,71,78,85} );
    }
  }

  SUBCASE("special cases") {
    // take all the values in the sample
    CHECK( uniform_ticks_in_sub_interval(0,11,11,11) == vector{0,1,2,3,4,5,6,7,8,9,10} );
    CHECK( uniform_ticks_in_sub_interval(0, 4,11,11) == vector{0,1,2,3} );
    CHECK( uniform_ticks_in_sub_interval(4,11,11,11) == vector{4,5,6,7,8,9,10} );

    // take all the values except one
    CHECK( uniform_ticks_in_sub_interval(0,11,11,10) == vector{0,2,3,4,5,6,7,8,9,10} );

    // one sample in one-size interval
    CHECK( uniform_ticks_in_sub_interval(0,1,1,1) == vector{0} );
  }

  SUBCASE("with empty sub-intervals") {
    CHECK( uniform_ticks_in_sub_interval(0,1,5,3) == vector{0} );
    CHECK( uniform_ticks_in_sub_interval(1,1,5,3) == vector<int>{} );
    CHECK( uniform_ticks_in_sub_interval(1,5,5,3) == vector{2,4} );
    CHECK( uniform_ticks_in_sub_interval(5,5,5,3) == vector<int>{} );
  }
}
