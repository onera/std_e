#include "doctest/doctest.h"
#include "std_e/algorithm/numerics.h"
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
