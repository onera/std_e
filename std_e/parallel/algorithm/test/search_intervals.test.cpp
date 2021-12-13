#include "std_e/unit_test/doctest_mpi.hpp"


#include "std_e/log.hpp"
#include "std_e/parallel/algorithm/search_intervals.hpp"

using namespace std_e;


TEST_CASE("uniform_partition_point") {
  int sz_tot = 1200;
  int n_interval = 12;

  for (int i=0; i<n_interval+1; ++i) {
    CHECK( uniform_partition_point(sz_tot,n_interval,i) == i*100 );
  }
}


TEST_CASE("search_intervals") {
  //       uniform_indices    0, 100  , 200 , 300, 400 ,  500  ,      600  ,  700  , 800, 900 , 1000  , 1100  ,  1200
  std::vector<int> indices = {0,    130, 201,          480, 530,550,580,  640,  701,         950,  1020,   1101 ,1200};

  SUBCASE("no tolerance") {
    auto [first_index, n_indices, interval_start] = search_intervals(indices,0.);

    std::vector<int> expected_first_index    = {/*0 is bound*/1,2,3,5,6,7,8,10,11/*12 is bound*/};
    std::vector<int> expected_n_indices      = {              1,1,2,1,1,1,2, 1, 1               };
    std::vector<int> expected_interval_start = {              0,1,2,3,6,7,8, 9,10               };

    CHECK( first_index    == expected_first_index );
    CHECK( n_indices      == expected_n_indices );
    CHECK( interval_start == expected_interval_start );
  }
  SUBCASE("tolerance") {
    auto [first_index, n_indices, interval_start] = search_intervals(indices,0.1);

    std::vector<int> expected_first_index    = {/*0 is bound*/1,/*2 is OK*/3,5,6,7,8,10/*11 is OK,12 is bound*/};
    std::vector<int> expected_n_indices      = {              1,           2,1,1,1,2, 1                        };
    std::vector<int> expected_interval_start = {              0,           2,3,6,7,8, 9                        };

    CHECK( first_index    == expected_first_index );
    CHECK( n_indices      == expected_n_indices );
    CHECK( interval_start == expected_interval_start );
  }
}
