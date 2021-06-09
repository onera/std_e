#include "std_e/unit_test/doctest.hpp"

#include "std_e/interval/algorithm.hpp"

using namespace std;
using namespace std_e;


TEST_CASE("is_interval") {
  vector<int> inter = {2,3,4,5};
  CHECK(is_interval(inter));

  vector<int> non_inter0 = {2,3,  5};
  CHECK_FALSE(is_interval(non_inter0));

  vector<int> non_inter1 = {2,3,4,5,4};
  CHECK_FALSE(is_interval(non_inter1));
}


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
