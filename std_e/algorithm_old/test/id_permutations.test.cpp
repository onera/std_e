#include "doctest/doctest.h"

#include "std_e/algorithm_old/id_permutations.hpp"

using namespace std;

TEST_CASE("permute_id") {
  const vector<int> permutation = {2,1,0};
  int offset = 42;

  int id;

  id = 42;
  std_e::permute_id(id,permutation,offset);
  CHECK( id == 44 );

  id = 43;
  std_e::permute_id(id,permutation,offset);
  CHECK( id == 43 );

  id = 44;
  std_e::permute_id(id,permutation,offset);
  CHECK( id == 42 );
}

TEST_CASE("permute_id_if_in_range") {
  vector<int> permutation = {2,1,0};
  std_e::interval<int> inter = {42,42+3};

  int id;

  id = 41;
  std_e::permute_id_if_in_range(id,permutation,inter);
  CHECK( id == 41 );


  id = 42;
  std_e::permute_id_if_in_range(id,permutation,inter);
  CHECK( id == 44 );

  id = 43;
  std_e::permute_id_if_in_range(id,permutation,inter);
  CHECK( id == 43 );

  id = 44;
  std_e::permute_id_if_in_range(id,permutation,inter);
  CHECK( id == 42 );


  id = 45;
  std_e::permute_id_if_in_range(id,permutation,inter);
  CHECK( id == 45 );
}

TEST_CASE("update_ids_after_permutation__involutive") {
  vector<int> permutation = {2,1,0}; // permutation is involutive
  int offset = 42;

  vector<int> ids = {42,44,43};

  std_e::update_ids_after_permutation(ids,permutation,offset);
  
  CHECK( ids == (vector<int>{44,42,43}) );
}

TEST_CASE("update_ids_after_permutation__non_involutive") {
  vector<int> permutation = {1,2,0}; // permutation is non involutive
  int offset = 42;

  vector<int> ids = {42,44,43};

  std_e::update_ids_after_permutation(ids,permutation,offset);
  
  CHECK( ids == (vector<int>{44,43,42}) );
}

TEST_CASE("update_ids_in_range_after_permutation__non_involutive") {
  vector<int> permutation = {1,2,0}; // permutation is non involutive
  std_e::interval<int> inter = {42,42+3};

  vector<int> ids = {41,42,44,43,45};

  std_e::update_ids_in_range_after_permutation(ids,permutation,inter);
  
  CHECK( ids == (vector<int>{41, 44,43,42, 45}) );
}
