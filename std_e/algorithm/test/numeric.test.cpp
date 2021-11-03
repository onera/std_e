#include "std_e/unit_test/doctest.hpp"
#include "std_e/algorithm/numeric.hpp"
#include <vector>

TEST_CASE("partial_accumulate") {
  std::vector<int> in = {2,3,4,5};
  std::vector<int> out(in.size() + 1);
  std_e::partial_accumulate(begin(in),end(in), begin(out), 7);

  std::vector<int> expected_out = {7, 7+2, 7+2+3, 7+2+3+4, 7+2+3+4+5};

  CHECK( out==expected_out );
}

TEST_CASE("partial_accumulate with user-defined reduction") {
  std::vector<std::pair<int,int>> in = {{2,6},{3,7},{4,8}};
  std::vector<int> out(in.size() + 1);

  auto reduction_fun = [](int i, const std::pair<int,int>& p){ return i + p.first*p.second; };

  std_e::partial_accumulate(begin(in),end(in), begin(out), 42, reduction_fun);

  std::vector<int> expected_out = {42,  42 +2*6,  42 +2*6 +3*7,  42 +2*6 +3*7 +4*8};

  CHECK( out==expected_out );
}

TEST_CASE("exclusive_adjacent_difference") {
  SUBCASE("base") {
    std::vector<int> v = {3,10,21,50};
    std::vector<int> res;

    std_e::exclusive_adjacent_difference(begin(v),end(v),std::back_inserter(res));

    REQUIRE( res.size() == 3 );
    CHECK( res[0] == 7 );
    CHECK( res[1] == 11 );
    CHECK( res[2] == 29 );
  }

  SUBCASE("one-elt range") {
    std::vector<int> v = {10};
    std::vector<int> res;

    std_e::exclusive_adjacent_difference(begin(v),end(v),std::back_inserter(res));

    REQUIRE( res.size() == 0 );
  }
}
