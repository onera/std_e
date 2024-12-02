#include "std_e/unit_test/doctest.hpp"

#include "std_e/algorithm/sort.hpp"
#include "std_e/debug.hpp"
#include <vector>
#include "std_e/data_structure/multi_range/multi_range.hpp"
#include "std_e/future/sort/sort_ranges.hpp"
#include <ranges>
using std::vector;

using namespace std_e;


TEST_CASE("sort_by_first vs indirect_sort") {
  SUBCASE("sort array with permutation") {
    vector<int> v    = {9,7,4,5};
    vector<int> perm = {0,3,1,2};

    // In this case, we sort the first array,
    // and the second "follows" where the elements of the first go
    ranges::sort(zip(v,perm), proj<0>);

    vector<int>    v_expected = {4,5,7,9};
    vector<int> perm_expected = {1,2,3,0};
    CHECK(    v ==    v_expected );
    CHECK( perm == perm_expected );
  }

  SUBCASE("sort array through permutation") {
    vector<int> v    = {9,7,4,5};
    vector<int> perm = {0,3,1,2};

    // In this case, we see array `v` throught an indirect numbering
    // That is, `v` is seen as {9,4,5,7}
    indirect_sort(v, perm);

    vector<int>    v_expected = {4,5,7,9};
    vector<int> perm_expected = {2,3,1,0};
    CHECK(    v ==    v_expected );
    CHECK( perm == perm_expected );
  }
}

TEST_CASE("sort_permutation and indirect_sort") {
  vector<int> v = {100, 90, 90, 100, 80, 80, 80, 70, 60};


  SUBCASE("sort_permutation") {
    vector<int> perm = std_e::sort_permutation(v);
    vector<int> perm_expected = {8, 7, 4, 5, 6, 1, 2, 0, 3};

    CHECK( perm == perm_expected );
  }

  SUBCASE("sort_permutation custom comp") {
    vector<int> perm = std_e::sort_permutation(v, std::greater<int>());
    vector<int> perm_expected = {0, 3, 1, 2, 4, 5, 6, 7, 8};

    CHECK( perm == perm_expected );
  }

  SUBCASE("indirect_sort") {
    vector<int> perm(v.size());
    std::iota(begin(perm), end(perm), 0);

    std_e::indirect_sort(v, perm);

    vector<int> v_expected = {60, 70, 80, 80, 80, 90, 90, 100, 100};
    vector<int> perm_expected = {8, 7, 4, 5, 6, 1, 2, 0, 3};

    CHECK( v == v_expected );
    CHECK( perm == perm_expected );
  }
}


TEST_CASE("zip_sort") {
  vector<int> v0 = {4 ,3 ,1 ,5 ,2 };
  vector<int> v1 = {40,30,10,50,20};

  auto perm = std_e::zip_sort(std::tie(v0,v1));

  CHECK( v0 == vector{1 ,2 ,3 ,4 ,5 } );
  CHECK( v1 == vector{10,20,30,40,50} );
  CHECK( perm == vector{2,4,1,0,3} );
}


TEST_CASE("sorted") {
  CHECK( sorted(vector<int>{4,3,1,5,2}) == vector<int>{1,2,3,4,5} );
}
