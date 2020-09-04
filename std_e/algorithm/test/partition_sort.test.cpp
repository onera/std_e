#include "std_e/unit_test/doctest.hpp"
#include "std_e/algorithm/partition_sort.hpp"
#include <vector>

using namespace std;

TEST_CASE("partition_sort") {
  vector<int> v = {110,200,2,6,-3,800,10001,999,8,50,0};

  vector<int> partition_values = {0,10,100,120,1000};

  SUBCASE("low-level algorithm") {
    using it_type = typename vector<int>::iterator;
    vector<it_type> partition_points(partition_values.size());
    std_e::partition_sort(begin(v),end(v),begin(partition_values),end(partition_values),begin(partition_points));

    //                    0        10   100    120            1000
    CHECK( v == vector{-3, 8,2,6,0,  50,   110,   999,800,200,     10001} );
  }
  SUBCASE("vector-returning algorithm") {
    auto partition_points = std_e::partition_sort(v,partition_values);
    //                    0        10   100    120            1000
    CHECK( v == vector{-3, 8,2,6,0,  50,   110,   999,800,200,     10001} );
  }
  SUBCASE("indices vector-returning algorithm") {
    auto partition_indices = std_e::partition_sort_indices(v,partition_values);
    //                                    0        10   100    120            1000
    CHECK(                 v == vector{-3, 8,2,6,0,  50,   110,   999,800,200,     10001} );
    CHECK( partition_indices == vector{    1      ,  5 ,   6  ,   7          ,     10   } );
  }
}

