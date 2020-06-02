#include "std_e/unit_test/doctest.hpp"

#include "std_e/interval/algorithm.hpp"

using namespace std;

TEST_CASE("rotated_position") {
  int first=10, n_first=14, last=20;

  //  10  11  12  13  14  15  16  17  18  19  20   (old positions)
  //  |______________||_____________________|
  //          ^                   ^
  //          |___________________|
  //
  //  14  15  16  17  18  19  10  11  12  13       (new positions)

  int r;
  
  r = std_e::rotated_position(10, first,n_first,last);
  CHECK( r == 16 );

  r = std_e::rotated_position(12, first,n_first,last);
  CHECK( r == 18 );

  r = std_e::rotated_position(15, first,n_first,last);
  CHECK( r == 11 );

  r = std_e::rotated_position(18, first,n_first,last);
  CHECK( r == 14 );

  r = std_e::rotated_position(19, first,n_first,last);
  CHECK( r == 15 );

  SUBCASE("not in interval") {
    r = std_e::rotated_position(7, first,n_first,last);
    CHECK( r == 7 );

    r = std_e::rotated_position(27, first,n_first,last);
    CHECK( r == 27 );
  }
}
