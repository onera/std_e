#include "std_e/unit_test/doctest.hpp"
#include "std_e/algorithm/block.hpp"
#include "std_e/future/dynarray.hpp"

using namespace std_e;

TEST_CASE("for_each_block") {
  std_e::dynarray<int> idx = {0          ,  6      , 10  ,12};
  std_e::dynarray<int>   x = {0,0,1,3,3,3,  3,4,4,5,  6,7};

  auto copy_two_first_odd_numbers = [](int* first, int* last, int* d_first) {
    int cnt = 0;
    while (first != last && cnt<2) {
      if (*first%2 == 1) {
        *d_first++ = *first;
        ++cnt;
      }
      ++first;
    }
    return d_first;
  };

  for_each_block(idx, x, copy_two_first_odd_numbers);
  CHECK( idx == std_e::dynarray<int>{0  , 2  , 4, 5} );
  CHECK(   x == std_e::dynarray<int>{1,3, 3,5, 7} );
}

TEST_CASE("sort_unique_by_block") {
  std_e::dynarray<int> idx = {0          ,  6      , 10  , 12};
  std_e::dynarray<int>   x = {0,0,1,3,3,3,  3,4,4,5,  6,7};
  sort_unique_by_block(idx, x);
  CHECK( idx == std_e::dynarray<int>{0    , 3    , 6  ,8} );
  CHECK(   x == std_e::dynarray<int>{0,1,3, 3,4,5, 6,7} );
}

TEST_CASE("remove_by_block") {
  std_e::dynarray<int> idx = {0          ,  6      , 10  , 12};
  std_e::dynarray<int>   x = {0,0,1,3,4,0,  1,0,1,5,  6,7};
  remove_by_block(idx, x, 0);
  CHECK( idx == std_e::dynarray<int>{0    , 3    , 6  ,8} );
  CHECK(   x == std_e::dynarray<int>{1,3,4, 1,1,5, 6,7} );
}


TEST_CASE("permute_by_block") {
  std_e::dynarray<int> idx = {0          ,  6      , 10  , 12};
  std_e::dynarray<int>   x = {0,0,1,3,3,3,  3,4,4,5,  6,7};

  std_e::dynarray<int> perm = {1,2,0};
  auto [p_idx, p_x] = std_e::permute_by_block(idx, x, perm);

  CHECK( p_idx == std_e::dynarray<int>{0      ,  4  ,  6          , 12} );
  CHECK(   p_x == std_e::dynarray<int>{3,4,4,5,  6,7,  0,0,1,3,3,3}    );
}
