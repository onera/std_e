#if __cplusplus > 201703L
#include "std_e/unit_test/doctest.hpp"
#include "std_e/unit_test/id_string.hpp"
#include "std_e/algorithm/partition_sort.hpp"
#include <vector>

using namespace std;
using namespace std_e;
using std_e::id_string;

// TODO rename file partition (since it does partition_sort + partition_indices)
// TODO rename partition_sort -> partition (extension of std::partition)
// TODO rename partition_sort_indices -> partition_is
// TODO rename partition_indices -> partition_point_is
// TODO create partition_point (extension of std::partition_point)
// TODO or rename partitions/multi_partition, partitions_i/multi_partition_i, partition_points, partition_points_i
TEST_CASE("partition_sort") {
  vector<int> v = {110,200,2,6,-3,800,10001,999,8,50,0};

  vector<int> partition_values = {0,10,100,120,1000};

  SUBCASE("low-level algorithm") {
    using it_type = typename vector<int>::iterator;
    vector<it_type> partition_points(partition_values.size());
    std_e::partition_sort(begin(v),end(v),begin(partition_values),end(partition_values),begin(partition_points));

    //                    0        10   100    120            1000
    CHECK( v == vector{-3, 8,2,6,0,  50,   110,   999,800,200,     10001} );
    CHECK( partition_points.size() == 5 );
    CHECK( partition_points[0] == begin(v)+1 );
    CHECK( partition_points[1] == begin(v)+5 );
    CHECK( partition_points[2] == begin(v)+6 );
    CHECK( partition_points[3] == begin(v)+7 );
    CHECK( partition_points[4] == begin(v)+10);
  }
  SUBCASE("vector-returning algorithm") {
    auto partition_points = std_e::partition_sort(v,partition_values);
    //                    0        10   100    120            1000
    CHECK( v == vector{-3, 8,2,6,0,  50,   110,   999,800,200,     10001} );
  }
  SUBCASE("indices vector-returning algorithm") {
    auto partition_indices = std_e::partition_sort_indices(v,partition_values);
    //                           0        10   100    120            1000
    CHECK(                 v == vector{-3, 8,2,6,0,  50,   110,   999,800,200,     10001} );
    CHECK( partition_indices == vector{ 0, 1      ,  5 ,   6  ,   7          ,     10   } );
  }

  SUBCASE("indirect algorithm") {
    auto [partition_indices,new_to_old] = std_e::indirect_partition_sort(v,partition_values);
    //                          0          10  100  120   1000
    CHECK(        new_to_old == vector{4,  8,2,3,10,  9,  0,  7,5,1, 6 } );
    CHECK( partition_indices == vector{0,  1       ,  5,  6,  7    , 10} );
  }

  SUBCASE("apply indirect algorithm") {
    auto [partition_indices,new_to_old] = std_e::apply_indirect_partition_sort(v,partition_values);
    //                          0           10  100  120   1000
    CHECK(                 v == vector{-3, 8,2,6, 0,  50,   110,   999,800,200,     10001} );
    CHECK(        new_to_old == vector{ 4, 8,2,3,10,   9,     0,     7,  5,  1,         6} );
    CHECK( partition_indices == vector{0,  1      ,  5 ,   6  ,   7          ,     10   } );
  }
}

TEST_CASE("partition_indices") {
  vector<int> v = {-3,  8,2,6,0,  50,  110,  999,800,200,  10001};
  vector<int> partition_values = {0,10,100,120,1000};

  auto partition_indices = std_e::partition_indices(v,partition_values);
  CHECK( partition_indices == vector{1,5,6,7,10} );
}

TEST_CASE("sort_into_partitions") {
  vector<id_string> v = {{30,"35"},{10,"11"},{30,"31"},{10,"10"},{10,"12"},{20,"23"},{20,"21"},{10,"13"},};
  std_e::jagged_vector<id_string> jv = std_e::sort_into_partitions(std::move(v),[](const id_string& x){ return x.id; });

  vector<id_string> expected_values = {{10,"11"},{10,"10"},{10,"12"},{10,"13"},{20,"23"},{20,"21"},{30,"35"},{30,"31"},};
  vector<int> expected_partition_indices = {0,4,6,8};
  CHECK( jv.flat_view() == expected_values );
  CHECK( jv.indices() == expected_partition_indices );
}
#endif // C++>17
