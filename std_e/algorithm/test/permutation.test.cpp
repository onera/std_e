#include "std_e/unit_test/doctest.hpp"

#include "std_e/algorithm/permutation.hpp"
#include <vector>
using std::vector;


TEST_CASE("inverse_permutation") {
  vector<int> perm = {4,3,0,1,2};

  vector<int> inv_perm = std_e::inverse_permutation(perm);

  vector<int> expected_inv_perm = {2,3,4,1,0};
  CHECK( inv_perm == expected_inv_perm );

  CHECK( std_e::inverse_permutation(std_e::inverse_permutation(perm)) == perm );
}


TEST_CASE("permute_copy_n") {
  int sz = 3;
  vector<double> v = { 3.14 , 2.7 , 6.67};
  vector<double> d_v(sz);
  vector<int> perm = {1,2,0};
  
  std_e::permute_copy_n(v.begin(),d_v.begin(),perm.begin(),sz);

  vector<double> expected_d_v = {2.7 , 6.67 , 3.14};
  CHECK( d_v == expected_d_v );
}

TEST_CASE("permute") {
  vector<double> v = { 3.14 , 2.7 , 6.67};
  vector<int> perm = {1,2,0};

  std_e::permute(v.begin(),perm);

  vector<double> expected_v = {2.7 , 6.67 , 3.14};
  CHECK( v == expected_v );
}


TEST_CASE("sort and permutations") {
  vector<int> v = {100, 90, 90, 100, 80, 80, 80, 70, 60};

  vector<int> perm1 = std_e::sort_permutation(v);
  vector<int> perm2 = std_e::sort_permutation(v, std::greater<int>());


  SUBCASE("sort_permutation") {
    vector<int> perm1_expected = {8, 7, 4, 5, 6, 1, 2, 0, 3};
    vector<int> perm2_expected = {0, 3, 1, 2, 4, 5, 6, 7, 8};

    CHECK( perm1 == perm1_expected );
    CHECK( perm2 == perm2_expected );
  }


  vector<int> p_v_expected = {60, 70, 80, 80, 80, 90, 90, 100, 100};

  SUBCASE("permute_copy") {
    auto p_v = std_e::permute_copy(v, perm1);

    CHECK( p_v == p_v_expected );
  }

  SUBCASE("permute") {
    std_e::permute(v, perm1);

    CHECK( v == p_v_expected );
  }
}
