#include "std_e/unit_test/doctest.hpp"

#include "std_e/algorithm/id_permutations.hpp"

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
  vector<int> permutation = {2,1,0}; // involutive permutation
  int offset = 42;

  vector<int> ids = {42,44,43};

  std_e::update_ids_after_permutation(ids,permutation,offset);

  CHECK( ids == (vector<int>{44,42,43}) );
}

TEST_CASE("update_ids_after_permutation__non_involutive") {
  vector<int> permutation = {2,0,1}; // non-involutive permutation
  int offset = 42;

  vector<int> ids = {42,44,43};

  std_e::update_ids_after_permutation(ids,permutation,offset);

  CHECK( ids == (vector<int>{44,43,42}) );
}


//// This test fails for specific compilation options
//// See https://github.com/onera/std_e/issues/4
//// Can't find what goes wrong, so commenting out for now
//TEST_CASE("update_ids_in_range_after_permutation__non_involutive") {
//  vector<int> permutation = {2,0,1}; // non-involutive permutation
//  std_e::interval<int> inter = {42,42+3};
//
//  vector<int> ids = {41, 42,44,43, 45};
//
//  std_e::update_ids_in_range_after_permutation(ids,permutation,inter);
//
//  CHECK( ids == (vector<int>{41, 44,43,42, 45}) );
//}


TEST_CASE("offset_permutation") {
  std_e::offset_permutation permutation(42, {2,1,0});

  CHECK( permutation(42) == 44 );
  CHECK( permutation(43) == 43 );
  CHECK( permutation(44) == 42 );
}


TEST_CASE("apply_permutation") {
  SUBCASE("involutive permutation") {
    std_e::offset_permutation permutation(42, {2,1,0}); // involutive permutation

    vector<int> ids = {42,44,43};

    std_e::apply(permutation,ids);

    CHECK( ids == (vector{44,42,43}) );
  }

  SUBCASE("non-involutive permutation") {
    std_e::offset_permutation permutation(42, {2,0,1}); // non-involutive permutation

    vector<int> ids = {42,44,43};

    std_e::apply(permutation,ids);

    CHECK( ids == (vector{44,43,42}) );
  }
}
