#include "std_e/unit_test/doctest.hpp"

#include "std_e/algorithm/rotate/rotate.hpp"
#include "std_e/future/span.hpp"
#include <vector>

using std::vector;

TEST_CASE("rotate_optim") {
  SUBCASE("size 3") {
    vector<int> v3 = {2,3,4};
    std_e::span<int,3> s3(v3.data());

    std_e::rotate_optim(s3,begin(s3)+1);
    CHECK( v3 == vector{3,4, 2} );
  }

  SUBCASE("size 4") {
    vector<int> v4 = {2,3,4,5};
    std_e::span<int,4> s4(v4.data());

    std_e::rotate_optim(s4,begin(s4)+2);
    CHECK( v4 == vector{4,5, 2,3} );
  }

  SUBCASE("size 7") {
    vector<int> v7 = {0,1,2,3,4,5,6};

    std_e::rotate_optim(v7,begin(v7)+3);
    CHECK( v7 == vector{3,4,5,6, 0,1,2} );
  }
}

TEST_CASE("rotate_min_first") {
  vector<int> v = {2,3,1,4};
  std_e::span<int,4> s(v.data());

  std_e::rotate_min_first(s);
  CHECK( v == vector{1,4, 2,3} );
}
