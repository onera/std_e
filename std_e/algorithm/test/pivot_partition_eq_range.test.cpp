#include "std_e/unit_test/doctest.hpp"
#include "std_e/unit_test/id_string.hpp"
#include "std_e/algorithm/pivot_partition_eq_range.hpp"
#include "std_e/log.hpp" // TODO

using namespace std;
using std_e::id_string;

// [Sphinx Doc] pivot_partition_eq {
TEST_CASE("pivot_partition_eq") {
  SUBCASE("simple") {
    vector<int> v = {1000,800,8,1000,50,0};

    SUBCASE("pivot 0") {
      auto [inf,sup] = std_e::pivot_partition_eq(begin(v),end(v),0);
      CHECK( inf-begin(v) == 0 );
      CHECK( sup-begin(v) == 1 );
    }
    SUBCASE("pivot 120") {
      auto [inf,sup] = std_e::pivot_partition_eq(begin(v),end(v),120);
      CHECK( inf-begin(v) == 3 );
      CHECK( sup-begin(v) == 3 );
    }
    SUBCASE("pivot 1000") {
      auto [inf,sup] = std_e::pivot_partition_eq(begin(v),end(v),1000);
      CHECK( inf-begin(v) == 4 );
      CHECK( sup-begin(v) == 6 );
    }
  }

  SUBCASE("more complex") {
    vector<int> v = {6,5,1,5,1,4,9,10,5,2,2,3};

    auto [inf,sup] = std_e::pivot_partition_eq(begin(v),end(v),5);
    CHECK( v == vector{3,1,1,4,2,2, 5,5,5, 10,9,6} );
    CHECK( inf-begin(v) == 6 );
    CHECK( sup-begin(v) == 6+3 );
  }
}
// [Sphinx Doc] pivot_partition_eq }

// [Sphinx Doc] pivot_partition_eq_indices range {
TEST_CASE("pivot_partition_eq_indices - range version") {
  vector<int> v = {110,200,2,6,-3,1000,800,10001,999,8,1000,50,0};

  const vector<int> pivots = {0,10,100,120,1000};

  auto [partition_is_inf,partition_is_sup] = std_e::pivot_partition_eq_indices(v,pivots);

  //                     0       10   100    120             1000
  CHECK( v == vector{-3, 0,6,2,8,  50,   110,   800,200,999, 1000,1000, 10001} );
  CHECK( partition_is_inf.size() == pivots.size() + 2 );
  CHECK( partition_is_sup.size() == pivots.size() + 2 );
  CHECK( partition_is_inf == std_e::interval_vector<int>{0,1,5,6,7,10,13} );
  CHECK( partition_is_sup == std_e::interval_vector<int>{0,2,5,6,7,12,13} );
}
// [Sphinx Doc] pivot_partition_eq_indices range }

// [Sphinx Doc] pivot_partition_eq_indices proj range {
TEST_CASE("pivot_partition_eq_indices - custom projector - range version") {
  vector<id_string> v = {{110,"A"},{200,"B"},{2,"C"},{6,"D"},{-3,"E"},{1000,"X"},
                         {800,"F"},{10001,"G"},{999,"H"},{8,"I"},{1000,"X"},{50,"J"},{0,"K"}};

  const vector<int> pivots = {0,10,100,120,1000};

  constexpr auto proj = [](const id_string& x){ return x.id; };

  auto [partition_is_inf,partition_is_sup] = std_e::pivot_partition_eq_indices(v,pivots,{},proj);

  CHECK( v == vector<id_string>{{-3,"E"}, {0,"K"},{6,"D"},{2,"C"},{8,"I"}, {50,"J"}, {110,"A"},
                                {800,"F"},{200,"B"},{999,"H"}, {1000,"X"},{1000,"X"}, {10001,"G"}} );
  CHECK( partition_is_inf.size() == pivots.size() + 2 );
  CHECK( partition_is_sup.size() == pivots.size() + 2 );
  CHECK( partition_is_inf == std_e::interval_vector<int>{0,1,5,6,7,10,13} );
  CHECK( partition_is_sup == std_e::interval_vector<int>{0,2,5,6,7,12,13} );
}
// [Sphinx Doc] pivot_partition_eq_indices proj range }

// [Sphinx Doc] pivot_partition_eq_indices ret range {
TEST_CASE("pivot_partition_eq_indices - custom return - range version") {
  vector<int> v = {110,200,2,6,-3,1000,800,10001,999,8,1000,50,0};

  const vector<int> pivots = {0,10,100,120,1000};

  auto [partition_is_inf,partition_is_sup] = std_e::pivot_partition_eq_indices(v,pivots,{},{},std::vector<long>{});

  static_assert(is_same_v<decltype(partition_is_inf),std::vector<long>>);
  static_assert(is_same_v<decltype(partition_is_sup),std::vector<long>>);
  CHECK( v == vector{-3, 0,6,2,8,  50,   110,   800,200,999, 1000,1000, 10001} );

  // TODO does not work with vector because of ctor from size incompatible with interval_vector (TODO: correct in interval_vector)
  //CHECK( partition_is_inf.size() == pivots.size() + 2 );
  //CHECK( partition_is_sup.size() == pivots.size() + 2 );
  //CHECK( partition_is_inf == vector<long>{0,1,5,6,7,10,13} );
  //CHECK( partition_is_sup == vector<long>{0,2,5,6,7,12,13} );
}
// [Sphinx Doc] pivot_partition_eq_indices ret range }
