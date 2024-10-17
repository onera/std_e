#include "std_e/unit_test/doctest.hpp"

#include "std_e/interval/algorithm.hpp"
#include "std_e/interval/interval_sequence.hpp"

using namespace std;
using namespace std_e;


TEST_CASE("rotated_position") {

  //  10  11  12  13  14  15  16  17  18  19  20   (old positions)
  //  |______________||_____________________|
  //
  int first=10, n_first=14, last=20;

  // swapped intervals
  //  |                      |               |
  //  14  15  16  17  18  19  10  11  12  13       (new positions)

  CHECK( rotated_position(10, first,n_first,last) == 16 );
  CHECK( rotated_position(11, first,n_first,last) == 17 );
  CHECK( rotated_position(12, first,n_first,last) == 18 );
  CHECK( rotated_position(13, first,n_first,last) == 19 );

  CHECK( rotated_position(14, first,n_first,last) == 10 );
  CHECK( rotated_position(15, first,n_first,last) == 11 );
  CHECK( rotated_position(16, first,n_first,last) == 12 );
  CHECK( rotated_position(17, first,n_first,last) == 13 );
  CHECK( rotated_position(18, first,n_first,last) == 14 );
  CHECK( rotated_position(19, first,n_first,last) == 15 );

  SUBCASE("not in interval") {
    CHECK( rotated_position(7, first,n_first,last) == 7 );
    CHECK( rotated_position(8, first,n_first,last) == 8 );
    CHECK( rotated_position(9, first,n_first,last) == 9 );

    CHECK( rotated_position(20, first,n_first,last) == 20 );
    CHECK( rotated_position(21, first,n_first,last) == 21 );
    CHECK( rotated_position(50, first,n_first,last) == 50 );
  }
}


TEST_CASE("minmax_over_sub_ranges and minmax_over_interval_sequence") {
  std::vector v = {5,3,0,  10,9,6,  11,12};

  SUBCASE("with non-empty intervals") {
    SUBCASE("indices") {
      auto [mins,maxs] = minmax_over_interval_sequence(v,vector{0,3,6,8});
      CHECK( mins == vector{0,6,11} );
      CHECK( maxs == vector{5,10,12} );
    }
    SUBCASE("iterators") {
      auto [mins,maxs] = minmax_over_sub_ranges(v,vector{begin(v),begin(v)+3,begin(v)+6,end(v)});
      CHECK( mins == vector{0,6,11} );
      CHECK( maxs == vector{5,10,12} );
    }
  }
  SUBCASE("with empty intervals") {
    auto [mins,maxs] = minmax_over_interval_sequence(v,vector{0,3,3,6,6,8});
    CHECK( mins == vector{0,6,11} );
    CHECK( maxs == vector{5,10,12} );
  }
}

TEST_CASE("is_partitioned_at and is_partitioned_at_indices") {
  std::vector v = {5,3,0,  10,9,6,  11,12};

  // we have {5,3,0} < {10,9,6} < {11,12}
  CHECK( is_partitioned_at_indices(v,vector{3,6}) );
  //CHECK( is_partitioned_at(v,vector{begin(v)+3,begin(v)+6}) );

  //// we don't have {5,3,0,10,9} < {6,11,12}
  //CHECK( !is_partitioned_at_indices(v,vector{5}) );
  //CHECK( !is_partitioned_at(v,vector{begin(v)+5}) );
}
