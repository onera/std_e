#include "doctest/doctest.h"

#include "std_e/algorithm_old/algorithm.hpp"

using namespace std;

TEST_CASE(test__std_e__algorithm__algorithm,permute_id) {
  const vector<index_t> involutive_perm = {2,1,0};

  auto inv0 = std_e::inverse_permutation(involutive_perm);

  CHECK( inv0  == involutive_perm );


  const vector<index_t> non_involutive_perm = {1,2,0};

  auto inv1 = std_e::inverse_permutation(non_involutive_perm);

  const vector<index_t> non_involutive_perm__expected_inverse = {2,0,1};
  CHECK( inv1 == non_involutive_perm__expected_inverse );
}

TEST_CASE(test__std_e__algorithm__algorithm,rotated_position) {
  index_t first=10, last=20, n_first=15;

  //  10  11  12  13  14  15  16  17  18  19  20   (old positions)
  //  |__________________||_________________|
  //          ^                   ^
  //          |___________________|
  //
  //  15  16  17  18  19  10  11  12  13  14    (new positions)

  index_t index, r;
  
  index = 10;
  r = std_e::rotated_position(index,first,n_first,last);
  CHECK( r == 15 );

  index = 12;
  r = std_e::rotated_position(index,first,n_first,last);
  CHECK( r == 17 );

  index = 15;
  r = std_e::rotated_position(index,first,n_first,last);
  CHECK( r == 10 );

  index = 18;
  r = std_e::rotated_position(index,first,n_first,last);
  CHECK( r == 13 );

  index = 19;
  r = std_e::rotated_position(index,first,n_first,last);
  CHECK( r == 14 );
}


TEST_CASE(test__std_e__algorithm__algorithm,permute_copy) {
  vector<double> v = { 3.14 , 2.7 , 6.67};
  vector<double> d_v(3);
  vector<index_t> perm = {1,2,0};
  
  std_e::permute_copy(v.begin(),d_v.begin(),perm);

  CHECK( d_v == (vector<double>{ 2.7 , 6.67 , 3.14 }) );
}

TEST_CASE(test__std_e__algorithm__algorithm,permute) {
  vector<double> v = { 3.14 , 2.7 , 6.67};
  vector<index_t> perm = {1,2,0};

  std_e::permute(v.begin(),perm);

  CHECK( v == (vector<double>{ 2.7 , 6.67 , 3.14 }) );
}
