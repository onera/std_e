#include "std_e/unit_test/doctest_mpi.hpp"


#include "std_e/log.hpp"
#include "std_e/parallel/algorithm/search_intervals.hpp"

using namespace std_e;
using std::vector;


TEST_CASE("search_intervals") {
  //                         0        1     2    3    4           5       6      7    8      9      10
  vector<int> ticks  =  {   100  ,   200 , 300, 400, 500  ,      600  ,  700  , 800, 900 , 1000  , 1100         };
  vector<int> x      = {0,    130,  199,          480, 530,550,580,  640, 701,         950,  1020,   1101 ,1200};
  //                    0      1     2             3    4   5   6     7    8            9     10      11    12


  auto [first_ticks, inter_indices] = search_intervals(ticks,x);

  CHECK( first_ticks   == vector{0, 1, 4, 5, 6, 7, 9, 10, 11} );
  CHECK( inter_indices == vector{0, 2, 3, 6, 7, 8, 9, 10, 11} );

  // Example of how to read it at position 5:
  CHECK( first_ticks[5]   == 7 ); // it means we are talking about `ticks[7]`...
  CHECK( first_ticks[5+1] == 9 ); // until `ticks[9]` (not included),
  // so we are talking about `ticks[7]` and `ticks[8]`
  // as a remainder, there values are the following:
  CHECK( ticks[7] == 800 );
  CHECK( ticks[8] == 900 );

  // inter_indices[5] gives us is the interval within `x` that contains 800 and 900:
  CHECK( inter_indices[5] == 8 );
  // interval number 8 is the following:
  CHECK( x[8]   == 701 );
  CHECK( x[8+1] == 950 );
  // and indeed, we have 701 <= 800 <= 900 < 950

  // if we write it with the returned arrays rather than the concrete values, we have:
  for (int j=first_ticks[5]; j<first_ticks[5+1]; ++j) { // for tick in [ticks[7],ticks[9])
      CHECK( x[inter_indices[5]] <= ticks[j]                         );
      CHECK(                        ticks[j] < x[inter_indices[5]+1] );
  }

  // We have done it for i=5, but it can be generalized for all i in [0,first_ticks.size()-1)
  for (int i=0; i<(int)first_ticks.size()-1; ++i) {
    for (int j=first_ticks[i]; j<first_ticks[i+1]; ++j) {
      CHECK( x[inter_indices[i]] <= ticks[j]                         );
      CHECK(                        ticks[j] < x[inter_indices[i]+1] );
    }
  }
}


TEST_CASE("search_near_or_containing_interval") {
  //                         0        1     2    3    4           5       6      7    8      9      10
  vector<int> ticks  =  {   100  ,   200 , 300, 400, 500  ,      600  ,  700  , 800, 900 , 1000  , 1100        };
  vector<int> x      = {0,    130,  199,          480, 530,550,580,  640, 701,         950,  1020,   1101 ,1200};
  //                    0      1     2             3    4   5   6     7    8            9     10      11    12


  auto [far_first_ticks,n_far_ticks,far_inter_indices,  near_tick_indices,near_inter_indices] = search_near_or_containing_interval(ticks,x,1);

  // same principle as search_intervals for `far_first_ticks` and `far_inter_indices`
  // but since now all the ticks are not necessarily here (the near ones are missing)
  // we need to know how many ticks there are in each interval, hence this `n_far_ticks` variable
  CHECK( far_first_ticks    == vector{0,2,4,5,7,9} );
  CHECK( n_far_ticks        == vector{1,2,1,1,2,1} );
  CHECK( far_inter_indices  == vector{0,2,3,6,8,9} );

  // if we take at position 4, we have [ticks[far_first_ticks[4]],ticks[far_first_ticks[4]+n_far_ticks[4]])
  // i.e. ticks[7] and ticks[8]
  // and these ones are in interval [x[far_inter_indices[4]],x[far_inter_indices[4]+1]),
  // that is, in interval [x[8],x[9])
  // Which we can verify: 701 <= 800 <= 900 < 950
  CHECK( x[8] <= ticks[7] ); CHECK( ticks[8] < x[9] );

  // refers to ticks indices:         200,700,1100
  CHECK( near_tick_indices  == vector{ 1 , 6,  10 } );
  // refers to x indices:             199,701,1101
  CHECK( near_inter_indices == vector{ 2 , 8 , 11} );
}
