#include "std_e/unit_test/doctest.hpp"
#include "std_e/utils/vector.hpp"


TEST_CASE("unique(vector)") {
  std::vector<int> v = {1,1,3,4,5,5,5,7};

  std_e::unique(v);

  std::vector<int> expected_v = {1,3,4,5,7};
  CHECK( v == expected_v );
}
TEST_CASE("sort_unique(vector)") {
  std::vector<int> v = {9,1,1,4,3,1,9,8,4,7};

  std_e::sort_unique(v);

  std::vector<int> expected_v = {1,3,4,7,8,9};
  CHECK( v == expected_v );
}
TEST_CASE("sort_unique_permutation(vector)") {
  std::vector<int> v = {9,1,1,4,3,1,9,8,4,7};

  std::vector<int> p = std_e::sort_unique_permutation(v);

  CHECK( p == std::vector{1,4,3,9,7,0} );

  SUBCASE("apply indirect sort unique == sort unique") {
    std::vector<int> sort_unique_v = v;
    std_e::sort_unique(sort_unique_v);

    std::vector<int> sort_unique_v_by_perm = std_e::permute_copy(v,p);

    CHECK( sort_unique_v_by_perm == sort_unique_v );
  }
}

TEST_CASE("std::vector sub_vector") {
  std::vector<int> a              = {3,4,5,6,7,8};
  std::vector<int> expected_sub_a =     {5,6,7};
  CHECK( std_e::make_sub_vector(a,2,3) == expected_sub_a );
}
