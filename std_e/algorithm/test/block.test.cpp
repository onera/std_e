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

  for_each_block(idx,x,copy_two_first_odd_numbers);
  CHECK( idx == std_e::dynarray<int>{0  , 2  , 4, 5} );
  CHECK(   x == std_e::dynarray<int>{1,3, 3,5, 7} );
}

TEST_CASE("sort_unique_each_block") {
  std_e::dynarray<int> idx = {0          ,  6      , 10  ,12};
  std_e::dynarray<int>   x = {0,0,1,3,3,3,  3,4,4,5,  6,7};
  sort_unique_each_block(idx,x);
  CHECK( idx == std_e::dynarray<int>{0    , 3    , 6  ,8} );
  CHECK(   x == std_e::dynarray<int>{0,1,3, 3,4,5, 6,7} );
}
