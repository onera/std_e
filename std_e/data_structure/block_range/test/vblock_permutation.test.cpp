#if __cplusplus > 201703L
#include "std_e/unit_test/doctest.hpp"

#include "std_e/data_structure/block_range/vblock_permutation.hpp"

using std::vector;


TEST_CASE("ngons") {
  // inputs
  vector<int> x =
    {  1, 2, 3,
      10,11,12,13,
       5, 4, 3,
       1, 8, 9 };
  vector<int> offsets = {0,3,7,10,13};
  auto bv = std_e::view_as_vblock_range(x,offsets);
  vector<int> my_permutation = {1,2, 0,3};

  // test
  std_e::permute_vblock_range(bv,my_permutation);

  // checks
  vector<int> expected_x = {
    10,11,12,13,
     5, 4, 3,
     1, 2, 3,
     1, 8, 9
  };
  vector<int> expected_offsets = {0,4,7,10,13};
  CHECK( x       == expected_x       );
  CHECK( offsets == expected_offsets );
}
#endif // C++>17
