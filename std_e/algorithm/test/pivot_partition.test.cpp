#if __cplusplus > 201703L
#include "std_e/unit_test/doctest.hpp"
#include "std_e/unit_test/id_string.hpp"
#include "std_e/algorithm/pivot_partition.hpp"
#include "std_e/interval/algorithm.hpp"
#include <vector>

using namespace std;
using std_e::id_string;



TEST_CASE("pivot_partition - simple test") {
  vector<int> v = {1000,800,8,1000,50,0};

  const vector<int> pivots = {0,120,1000};

  using it_type = typename vector<int>::iterator;
  vector<it_type> partition_pts(pivots.size());
  std_e::pivot_partition(begin(v),end(v),
                         begin(pivots),end(pivots),
                         begin(partition_pts));

  CHECK( partition_pts[0]-begin(v) == 0 ); // no elt less than 0
  CHECK( partition_pts[1]-begin(v) == 3 ); // 3 elts less than 120:  0,8,50
  CHECK( partition_pts[2]-begin(v) == 4 ); // 4 elts less than 1000: 0,8,50,800

  CHECK( std_e::is_partitioned_at(v,partition_pts) );

}

//TEST_CASE("pivot_partition - 2") {
//  vector<int> v = {13,11,10,14,0,7,9,6};
//  //vector<int> v = {12,3,4,8,5,1,2};
//
//  const vector<int> pivots = {8,11,15};
//
//  using it_type = typename vector<int>::iterator;
//  vector<it_type> partition_pts(pivots.size());
//  std_e::pivot_partition(begin(v),end(v),
//                         begin(pivots),end(pivots),
//                         begin(partition_pts));
//  ELOG(v);
//  ELOG(partition_pts[0]-begin(v));
//  ELOG(partition_pts[1]-begin(v));
//  ELOG(partition_pts[2]-begin(v));
//  CHECK( partition_pts[0]-begin(v) == 3 );
//  CHECK( partition_pts[1]-begin(v) == 5 );
//  CHECK( partition_pts[2]-begin(v) == 8 );
//
//  CHECK( std_e::is_partitioned_at(v,partition_pts) );
//}


// [Sphinx Doc] pivot_partition {
TEST_CASE("pivot_partition") {
  vector<int> v = {110,200,2,6,-3,800,10001,999,8,50,0};

  const vector<int> pivots = {0,10,100,120,1000};

  using it_type = typename vector<int>::iterator;
  vector<it_type> partition_pts(pivots.size());
  std_e::pivot_partition(begin(v),end(v),
                         begin(pivots),end(pivots),
                         begin(partition_pts));

  //                    0        10   100    120            1000
  CHECK( v == vector{-3, 8,2,6,0,  50,   110,   999,800,200,     10001} );
  CHECK( partition_pts[0] == begin(v)+1 );
  CHECK( partition_pts[1] == begin(v)+5 );
  CHECK( partition_pts[2] == begin(v)+6 );
  CHECK( partition_pts[3] == begin(v)+7 );
  CHECK( partition_pts[4] == begin(v)+10);
}
// [Sphinx Doc] pivot_partition }

// [Sphinx Doc] pivot_partition_point {
TEST_CASE("pivot_partition_point") {
  const vector pivots = {0,       10,  100,   120,           1000};
  const vector v =   {-3, 8,2,6,0,  50,   110,   999,800,200,     10001};

  using it_type = typename vector<int>::const_iterator;
  vector<it_type> partition_pts(pivots.size());
  std_e::pivot_partition_point(begin(v),end(v),
                               begin(pivots),end(pivots),
                               begin(partition_pts));

  CHECK( partition_pts[0] == begin(v)+1 );
  CHECK( partition_pts[1] == begin(v)+5 );
  CHECK( partition_pts[2] == begin(v)+6 );
  CHECK( partition_pts[3] == begin(v)+7 );
  CHECK( partition_pts[4] == begin(v)+10);
}
// [Sphinx Doc] pivot_partition_point }

// [Sphinx Doc] pivot_partition_indices {
TEST_CASE("pivot_partition_indices") {
  vector<int> v = {110,200,2,6,-3,800,10001,999,8,50,0};

  const vector<int> pivots = {0,10,100,120,1000};

  vector<int> partition_pts(pivots.size());
  std_e::pivot_partition_indices(begin(v),end(v),
                                 begin(pivots),end(pivots),
                                 begin(partition_pts));

  //                    0        10   100    120            1000
  CHECK( v == vector{-3, 8,2,6,0,  50,   110,   999,800,200,     10001} );
  CHECK( partition_pts[0] == 1 );
  CHECK( partition_pts[1] == 5 );
  CHECK( partition_pts[2] == 6 );
  CHECK( partition_pts[3] == 7 );
  CHECK( partition_pts[4] == 10);
}
// [Sphinx Doc] pivot_partition_indices }

// [Sphinx Doc] pivot_partition_point_indices {
TEST_CASE("pivot_partition_point_indices") {
  const vector pivots = {0,       10,  100,   120,           1000};
  const vector v =   {-3, 8,2,6,0,  50,   110,   999,800,200,     10001};

  vector<int> partition_pts(pivots.size());
  std_e::pivot_partition_point_indices(begin(v),end(v),
                                       begin(pivots),end(pivots),
                                       begin(partition_pts));

  CHECK( partition_pts[0] == 1 );
  CHECK( partition_pts[1] == 5 );
  CHECK( partition_pts[2] == 6 );
  CHECK( partition_pts[3] == 7 );
  CHECK( partition_pts[4] == 10);
}
// [Sphinx Doc] pivot_partition_point_indices }
#endif // __cplusplus > 201703L
