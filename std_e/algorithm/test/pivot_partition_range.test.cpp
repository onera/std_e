#include "std_e/unit_test/doctest.hpp"
#include "std_e/unit_test/id_string.hpp"
#include "std_e/algorithm/pivot_partition_range.hpp"

using namespace std;
using std_e::id_string;

//
// [Sphinx Doc] pivot_partition range {
TEST_CASE("pivot_partition - range version") {
  vector<int> v = {110,200,2,6,-3,800,10001,999,8,50,0};

  const vector<int> pivots = {0,10,100,120,1000};

  using it_type = typename vector<int>::iterator;
  std_e::interval_vector<it_type> partition_pts = std_e::pivot_partition(v,pivots);

  //                    0        10   100    120            1000
  CHECK( v == vector{-3, 8,2,6,0,  50,   110,   999,800,200,     10001} );
  CHECK( partition_pts.size() == pivots.size() + 2 );
  CHECK( partition_pts[0] == begin(v) );
  CHECK( partition_pts[1] == begin(v)+1 );
  CHECK( partition_pts[2] == begin(v)+5 );
  CHECK( partition_pts[3] == begin(v)+6 );
  CHECK( partition_pts[4] == begin(v)+7 );
  CHECK( partition_pts[5] == begin(v)+10);
  CHECK( partition_pts[6] == end(v));
}
// [Sphinx Doc] pivot_partition range }

// [Sphinx Doc] pivot_partition_point range {
TEST_CASE("pivot_partition_point - range version") {
  const vector pivots = {0,       10,  100,   120,           1000};
  const vector v =   {-3, 8,2,6,0,  50,   110,   999,800,200,     10001};

  using it_type = typename vector<int>::const_iterator;
  std_e::interval_vector<it_type> partition_pts = std_e::pivot_partition_point(v,pivots);

  CHECK( partition_pts.size() == pivots.size() + 2 );
  CHECK( partition_pts[0] == begin(v) );
  CHECK( partition_pts[1] == begin(v)+1 );
  CHECK( partition_pts[2] == begin(v)+5 );
  CHECK( partition_pts[3] == begin(v)+6 );
  CHECK( partition_pts[4] == begin(v)+7 );
  CHECK( partition_pts[5] == begin(v)+10);
  CHECK( partition_pts[6] == end(v));
}
// [Sphinx Doc] pivot_partition_point range }

// [Sphinx Doc] pivot_partition_indices range {
TEST_CASE("pivot_partition_indices - range version") {
  vector<int> v = {110,200,2,6,-3,800,10001,999,8,50,0};

  const vector<int> pivots = {0,10,100,120,1000};

  std_e::interval_vector<int> partition_is = std_e::pivot_partition_indices(v,pivots);

  //                    0        10   100    120            1000
  CHECK( v == vector{-3, 8,2,6,0,  50,   110,   999,800,200,     10001} );
  CHECK( partition_is.size() == pivots.size() + 2 );
  CHECK( partition_is[0] == 0 );
  CHECK( partition_is[1] == 1 );
  CHECK( partition_is[2] == 5 );
  CHECK( partition_is[3] == 6 );
  CHECK( partition_is[4] == 7 );
  CHECK( partition_is[5] == 10);
  CHECK( partition_is[6] == 11);
}
// [Sphinx Doc] pivot_partition_indices range }

// [Sphinx Doc] pivot_partition_point_indices range {
TEST_CASE("pivot_partition_point_indices - range version") {
  const vector pivots = {0,       10,  100,   120,           1000};
  const vector v =   {-3, 8,2,6,0,  50,   110,   999,800,200,     10001};

  std_e::interval_vector<int> partition_is = std_e::pivot_partition_point_indices(v,pivots);

  CHECK( partition_is.size() == pivots.size() + 2 );
  CHECK( partition_is[0] == 0 );
  CHECK( partition_is[1] == 1 );
  CHECK( partition_is[2] == 5 );
  CHECK( partition_is[3] == 6 );
  CHECK( partition_is[4] == 7 );
  CHECK( partition_is[5] == 10);
  CHECK( partition_is[6] == 11);
}
// [Sphinx Doc] pivot_partition_point_indices range }

// [Sphinx Doc] pivot_partition_indices comp range {
TEST_CASE("pivot_partition_indices - custom comparator - range version") {
  vector<id_string> v = {{110,"A"},{200,"B"},{2,"C"},{6,"D"},{-3,"E"},{800,"F"},{10001,"G"},{999,"H"},{8,"I"},{50,"J"},{0,"K"}};

  const vector<int> pivots = {0,10,100,120,1000};

  constexpr auto proj = [](const id_string& x){ return x.id; };

  std_e::interval_vector<int> partition_is = std_e::pivot_partition_indices(v,pivots,proj);

  //                                    0                                10       100         120                           1000
  CHECK( v == vector<id_string>{{-3,"E"}, {8,"I"},{2,"C"},{6,"D"},{0,"K"}, {50,"J"}, {110,"A"}, {999,"H"},{800,"F"},{200,"B"}, {10001,"G"}} );
  CHECK( partition_is.size() == pivots.size() + 2 );
  CHECK( partition_is[0] == 0 );
  CHECK( partition_is[1] == 1 );
  CHECK( partition_is[2] == 5 );
  CHECK( partition_is[3] == 6 );
  CHECK( partition_is[4] == 7 );
  CHECK( partition_is[5] == 10);
  CHECK( partition_is[6] == 11);
}
// [Sphinx Doc] pivot_partition_indices comp range }

// [Sphinx Doc] pivot_partition_indices ret range {
TEST_CASE("pivot_partition_indices - custom return - range version") {
  vector<int> v = {110,200,2,6,-3,800,10001,999,8,50,0};

  const vector<int> pivots = {0,10,100,120,1000};

  std::vector<long> partition_is = std_e::pivot_partition_indices(v,pivots,{},{},std::vector<long>{});

  //                    0        10   100    120            1000
  CHECK( v == vector{-3, 8,2,6,0,  50,   110,   999,800,200,     10001} );
  CHECK( partition_is.size() == pivots.size() + 2 );
  CHECK( partition_is[0] == 0l );
  CHECK( partition_is[1] == 1l );
  CHECK( partition_is[2] == 5l );
  CHECK( partition_is[3] == 6l );
  CHECK( partition_is[4] == 7l );
  CHECK( partition_is[5] == 10l);
  CHECK( partition_is[6] == 11l);
}
// [Sphinx Doc] pivot_partition_indices ret range }


  //SUBCASE("indirect algorithm") {
  //  auto [partition_indices,new_to_old] = std_e::indirect_partition_sort(v,pivots);
  //  //                                   0          10  100  120   1000
  //  CHECK(        new_to_old ==          vector{4,  8,2,3,10,  9,  0,  7,5,1, 6 } );
  //  CHECK( partition_indices == interval_vector{0,  1       ,  5,  6,  7    , 10} );
  //}

  //SUBCASE("apply indirect algorithm") {
  //  auto [partition_indices,new_to_old] = std_e::apply_indirect_partition_sort(v,pivots);
  //  //                                   0           10  100  120   1000
  //  CHECK(                 v ==          vector{-3, 8,2,6, 0,  50,   110,   999,800,200,     10001} );
  //  CHECK(        new_to_old ==          vector{ 4, 8,2,3,10,   9,     0,     7,  5,  1,         6} );
  //  CHECK( partition_indices == interval_vector{0,  1      ,  5 ,   6  ,   7          ,     10   } );
  //}

//TEST_CASE("sort_into_partitions") {
//  vector<id_string> v = {{30,"35"},{10,"11"},{30,"31"},{10,"10"},{10,"12"},{20,"23"},{20,"21"},{10,"13"},};
//  std_e::jagged_vector<id_string> jv = std_e::sort_into_partitions(std::move(v),[](const id_string& x){ return x.id; });
//
//  vector<id_string> expected_values = {{10,"11"},{10,"10"},{10,"12"},{10,"13"},{20,"23"},{20,"21"},{30,"35"},{30,"31"},};
//  std_e::interval_vector<int> expected_partition_indices = {0,4,6,8};
//  CHECK( jv.flat_view() == expected_values );
//  CHECK( jv.indices() == expected_partition_indices );
//}
