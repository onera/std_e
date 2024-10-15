#include "std_e/unit_test/doctest.hpp"

#include "std_e/algorithm/permutation.hpp"
#include <vector>
using std::vector;


TEST_CASE("inverse_permutation") {
  vector<int> perm = {4,3,0,1,2};

  vector<int> inv_perm = std_e::inverse_permutation(perm);

  vector<int> expected_inv_perm = {2,3,4,1,0};
  CHECK( inv_perm == expected_inv_perm );
}

TEST_CASE("inverse_partial_permutation") {
  int size = 5;
  vector<int> perm = {4,0,3};

  /*
      [A][B][C]
       \  |  |
        \_|__|___     content of 0 (i.e. A) goes into 4
          /  \   \    content of 1 (i.e. B) goes into 0
         /    \   \   content of 2 (i.e. C) goes into 3
        /      \   \
      [B][_][_][C][A]
  */

  vector<int> inv_perm = std_e::inverse_partial_permutation(perm,size,-1);

  vector<int> expected_inv_perm = {1,-1,-1,2,0};
  CHECK( inv_perm == expected_inv_perm );
}


TEST_CASE("compose_permutations") {
  vector<int> id = {0,1,2,3};
  vector<int> perm0 = {1,2,3,0};
  CHECK( std_e::compose_permutations(id,perm0) == perm0 );
  CHECK( std_e::compose_permutations(perm0,id) == perm0 );


  vector<int> perm0perm0 = {2,3,0,1};
  CHECK( std_e::compose_permutations(perm0,perm0) == perm0perm0 );

  vector<int> perm1 = {0,0,1,1};
  vector<int> perm0perm1 = {0,1,1,0}; // 0->1->0, 1->2->1, 2->3->1, 3->0->0
  CHECK( std_e::compose_permutations(perm0,perm1) == perm0perm1 );

  vector<int> perm1perm0 = {1,1,2,2}; // 0->0->1, 1->0->1, 2->1->2, 3->1->2
  CHECK( std_e::compose_permutations(perm1,perm0) == perm1perm0 );
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


TEST_CASE("unique permutations") {
  vector<int> v = {60, 70, 80, 80, 80, 90, 90, 100, 100};

  vector<int> perm = std_e::unique_permutation(v);

  vector<int> perm_expected = {0, 1, 2, 5, 7, -1, -1, -1, -1};

  CHECK( perm == perm_expected );
}
