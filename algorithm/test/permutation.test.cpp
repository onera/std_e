#include "doctest/doctest.h"
#include "doctest/doctest_utils.hpp"

#include "std_e/algorithm/permutation.hpp"
#include <vector>

TEST_CASE("invert_permutation") {
  std::vector<int> perm = {4,3,0,1,2};

  std::vector<int> inv_perm = std_e::invert_permutation(perm);

  std::vector<int> expected_inv_perm = {2,3,4,1,0};
  CHECK( inv_perm == expected_inv_perm );

  CHECK( std_e::invert_permutation(std_e::invert_permutation(perm)) == perm );
}

TEST_CASE("sort and permutations") {
  std::vector<int> v = {100, 90, 90, 100, 80, 80, 80, 70, 60};

  std::vector<int> perm1 = std_e::sort_permutation(v);
  std::vector<int> perm2 = std_e::sort_permutation(v, std::greater<int>());


  SUBCASE("sort_permutation") {
    std::vector<int> perm1_expected = {8, 7, 4, 5, 6, 1, 2, 0, 3};
    std::vector<int> perm2_expected = {0, 3, 1, 2, 4, 5, 6, 7, 8};

    CHECK( perm1 == perm1_expected );
    CHECK( perm2 == perm2_expected );
  }


  std::vector<int> p_v_expected = {60, 70, 80, 80, 80, 90, 90, 100, 100};

  SUBCASE("permute_copy") {
    auto p_v = std_e::permute_copy(v, perm1);

    CHECK( p_v == p_v_expected );
  }

  SUBCASE("permute") {
    std_e::permute(v, perm1);

    CHECK( v == p_v_expected );
  }
}
