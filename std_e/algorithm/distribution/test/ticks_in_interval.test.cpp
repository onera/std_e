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
  // position 3 samples in [0,22,54,60,85) :
  // 0-------------------22--------------------------------54------60-----------------------85
  //                      ^                       ^                      ^
  //                     22                      43                     64
  CHECK( uniform_ticks_in_sub_interval( 0,22,85,3) == vector<int>{} );
  CHECK( uniform_ticks_in_sub_interval(22,54,85,3) == vector{22,43} );
  CHECK( uniform_ticks_in_sub_interval(54,60,85,3) == vector<int>{} );
  CHECK( uniform_ticks_in_sub_interval(60,85,85,3) == vector{64} );

  // position 16 samples (exact division)
  CHECK( uniform_ticks_in_sub_interval( 0,22,85,16) == vector{5,10,15,20} );
  CHECK( uniform_ticks_in_sub_interval(22,54,85,16) == vector{25,30,35,40,45,50} );
  CHECK( uniform_ticks_in_sub_interval(54,60,85,16) == vector{55} );
  CHECK( uniform_ticks_in_sub_interval(60,85,85,16) == vector{60,65,70,75,80} );

  // position 10 samples
  CHECK( uniform_ticks_in_sub_interval( 0,22,85,10) == vector{8,16} );
  CHECK( uniform_ticks_in_sub_interval(22,54,85,10) == vector{24,32,40,48} );
  CHECK( uniform_ticks_in_sub_interval(54,60,85,10) == vector{56} );
  CHECK( uniform_ticks_in_sub_interval(60,85,85,10) == vector{64,71,78} );
  //
  // position 1 sample
  CHECK( uniform_ticks_in_sub_interval( 0,22,85,1) == vector<int>{} );
  CHECK( uniform_ticks_in_sub_interval(22,54,85,1) == vector{43} );
  CHECK( uniform_ticks_in_sub_interval(54,60,85,1) == vector<int>{} );
  CHECK( uniform_ticks_in_sub_interval(60,85,85,1) == vector<int>{} );

  // position 0 sample
  CHECK( uniform_ticks_in_sub_interval( 0,22,85,0) == vector<int>{} );
  CHECK( uniform_ticks_in_sub_interval(22,54,85,0) == vector<int>{} );
  CHECK( uniform_ticks_in_sub_interval(54,60,85,0) == vector<int>{} );
  CHECK( uniform_ticks_in_sub_interval(60,85,85,0) == vector<int>{} );


  // with an empty sub-interval
  CHECK( uniform_ticks_in_sub_interval(22,22,85,3) == vector<int>{} );

  // with the sub-interval being the complete interval
  CHECK( uniform_ticks_in_sub_interval(0,85,85,3) == vector{22,43,64} );

  // with the complete interval being too small:
  //   uniform_ticks_in_sub_interval(0,4,4,10) => precondition failure : n_ticks <= sz , but 10 > 4
}
