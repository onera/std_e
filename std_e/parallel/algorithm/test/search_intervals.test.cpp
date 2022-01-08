#include "std_e/unit_test/doctest_mpi.hpp"


#include "std_e/log.hpp"
#include "std_e/parallel/algorithm/search_intervals.hpp"

using namespace std_e;
using std::vector;


TEST_CASE("search_intervals") {
  vector<int> indices = {0,    130, 201,          480, 530,550,580,  640,  701,         950,  1020,   1101 ,1200};
  vector<int> ticks  =  {   100  , 200 , 300, 400 ,  500  ,      600  ,  700  , 800, 900 , 1000  , 1100         };

  SUBCASE("no tolerance") {
    auto [first_index, n_indices, interval_start] = search_intervals6(ticks,indices,0);

    vector<int> expected_first_index    = {/*0 is bound*/1,2,3,5,6,7,8,10,11/*12 is bound*/};
    vector<int> expected_n_indices      = {              1,1,2,1,1,1,2, 1, 1               };
    vector<int> expected_interval_start = {              0,1,2,3,6,7,8, 9,10               };

    CHECK( first_index    == expected_first_index );
    CHECK( n_indices      == expected_n_indices );
    CHECK( interval_start == expected_interval_start );
  }
  SUBCASE("tolerance") {
    // 201 and 1101 are wrong by 1, so they fall within a tolerance of 1
    // notice that 701 is at index 8, so it is not wrong by 701-700=1, but by 800-701=99, so not within the tolerance
    auto [first_index, n_indices, interval_start] = search_intervals6(ticks,indices,1);

    vector<int> expected_first_index    = {/*0 is bound*/1,/*2 is OK*/3,5,6,7,8,10/*11 is OK,12 is bound*/};
    vector<int> expected_n_indices      = {              1,           2,1,1,1,2, 1                        };
    vector<int> expected_interval_start = {              0,           2,3,6,7,8, 9                        };

    CHECK( first_index    == expected_first_index );
    CHECK( n_indices      == expected_n_indices );
    CHECK( interval_start == expected_interval_start );
  }
}


//TEST_CASE("search_intervals4") {
//  vector<int> ticks  =  {   100  , 200 , 300, 400 ,  500  ,      600  ,  700  , 800, 900 , 1000  , 1100         };
//  vector<int> indices = {0,    130, 201,          480, 530,550,580,  640,  701,         950,  1020,   1101 ,1200};
//
//  //SUBCASE("no tolerance") {
//  //  auto [first_index, n_indices, interval_start, found_ticks] = search_intervals4(ticks,indices,0);
//  //  ELOG(first_index);
//  //  ELOG(n_indices);
//  //  ELOG(interval_start);
//  //  ELOG(found_ticks);
//
//  //  vector<int> expected_first_index    = {0,1,2,4,5,6,7,9,10};
//  //  vector<int> expected_n_indices      = {1,1,2,1,1,1,2,1, 1};
//  //  vector<int> expected_interval_start = {0,1,2,3,6,7,8, 9,10};
//  //  vector<int> expected_found_ticks    = {               };
//
//  //  //CHECK( first_index    == expected_first_index );
//  //  //CHECK( n_indices      == expected_n_indices );
//  //  //CHECK( interval_start == expected_interval_start );
//  //  //CHECK( found_ticks    == expected_found_ticks);
//  //}
//  SUBCASE("tolerance") {
//    // 201 and 1101 are wrong by 1, so they fall within a tolerance of 1
//    // notice that 701 is at index 8, so it is not wrong by 701-700=1, but by 800-701=99, so not within the tolerance
//    auto [first_tick_indices, n_indices, interval_start, found_ticks] = search_intervals4(ticks,indices,2);
//    ELOG(first_tick_indices);
//    ELOG(n_indices);
//    ELOG(interval_start);
//    ELOG(found_ticks);
//
//
//   //first_index =    {0,1,2, 4,5,6,7, 9,10}
//   //n_indices =      {1,1,2, 1,1,1,2, 1,1}
//   //interval_start = {0,1,2,3,6,7,8,9,10}
//   //found_ticks = {}
//
//    vector<int> expected_first_index    = {/*0 is bound*/1,/*2 is OK*/3,5,6,7,8,10/*11 is OK,12 is bound*/};
//    vector<int> expected_n_indices      = {              1,           2,1,1,1,2, 1                        };
//    vector<int> expected_interval_start = {              0,           2,3,6,7,8, 9                        };
//    vector<int> expected_found_ticks    = {                             };
//
//    //CHECK( first_index    == expected_first_index );
//    //CHECK( n_indices      == expected_n_indices );
//    //CHECK( interval_start == expected_interval_start );
//    //CHECK( found_ticks    == expected_found_ticks);
//  }
//}

//TEST_CASE("search_intervals4") {
//  vector<int> indices = {0,4,23};
//  vector<int> obj_ticks = {4};
//
//  auto [first_index, n_indices, interval_start, index_ticks_found] = search_intervals4(obj_ticks,indices,1);
//  CHECK( first_index       == vector{0} );
//  CHECK( n_indices         == vector{1} );
//  CHECK( interval_start    == vector{1} );
//  CHECK( index_ticks_found == vector<int>{} );
//}


TEST_CASE("search_intervals7") {
  //                         0      1     2    3      4           5       6      7    8      9      10
  vector<int> ticks  =  {   100  , 200 , 300, 400 ,  500  ,      600  ,  700  , 800, 900 , 1000  , 1100         };
  vector<int> x      = {0,    130,  201,          480, 530,550,580,  640, 701,         950,  1020,   1101 ,1200};
  //                    0      1     2             3    4   5   6     7    8            9     10      11    12


  auto [first_ticks, inter_indices] = search_intervals7(ticks,x);

  CHECK( first_ticks   == vector{0, 1, 2, 4, 5, 6, 7, 9, 10, 11} );
  CHECK( inter_indices == vector{0, 1, 2, 3, 6, 7, 8, 9, 10, 11} );

  // Example of how to read it at position 6:
  CHECK( first_ticks[6]   == 7 ); // it means we are talking about `ticks[7]`...
  CHECK( first_ticks[6+1] == 9 ); // until `ticks[9]` (not included),
  // so we are talking about `ticks[7]` and `ticks[8]`
  // as a remainder, there values are the following:
  CHECK( ticks[7] == 800 );
  CHECK( ticks[8] == 900 );

  // inter_indices[6] gives us is the interval within `x` that contains 800 and 900:
  CHECK( inter_indices[6] == 8 );
  // interval number 8 is the following:
  CHECK( x[8]   == 701 );
  CHECK( x[8+1] == 950 );
  // and indeed, we have 701 <= 800 <= 900 < 950

  // if we write it with the returned arrays rather than the concrete values, we have:
  for (int j=first_ticks[6]; j<first_ticks[6+1]; ++j) { // for tick in [ticks[7],ticks[9])
      CHECK( x[inter_indices[6]] <= ticks[j]                         );
      CHECK(                        ticks[j] < x[inter_indices[6]+1] );
  }

  // We have done it for i=6, but it can be generalized for all i in [0,first_ticks.size()-1)
  for (int i=0; i<(int)first_ticks.size()-1; ++i) {
    for (int j=first_ticks[i]; j<first_ticks[i+1]; ++j) {
      CHECK( x[inter_indices[i]] <= ticks[j]                         );
      CHECK(                        ticks[j] < x[inter_indices[i]+1] );
    }
  }
}
