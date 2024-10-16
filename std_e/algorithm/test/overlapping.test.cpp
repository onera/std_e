#include "std_e/unit_test/doctest.hpp"
#include "std_e/algorithm/overlapping.hpp"
#include <vector>

using namespace std_e;

TEST_CASE("unique") {
  SUBCASE("with duplicates") {
    std_e::dynarray<int> x = {0,0,1,3,3,3, 3,4,4,5, 6,7};
    std_e::dynarray<int> res(x.size());
    auto res_ptr = res.data();
    res_ptr = unique(x.data()   , x.data()+ 6, res_ptr);
    res_ptr = unique(x.data()+ 6, x.data()+10, res_ptr);
    res_ptr = unique(x.data()+10, x.data()+12, res_ptr);
    auto final_sz = res_ptr - res.data();
    res.resize(final_sz);
    CHECK( res == std_e::dynarray<int>{0,1,3, 3,4,5, 6,7} );
  }
  SUBCASE("without duplicates") {
    std_e::dynarray<int> x = {0,1,3, 3,4,5, 6,7};
    std_e::dynarray<int> res(x.size());
    auto res_ptr = res.data();
    res_ptr = unique(x.data()   , x.data()+ 3, res_ptr);
    res_ptr = unique(x.data()+ 3, x.data()+ 6, res_ptr);
    res_ptr = unique(x.data()+ 6, x.data()+ 8, res_ptr);
    auto final_sz = res_ptr - res.data();
    res.resize(final_sz);
    CHECK( res == std_e::dynarray<int>{0,1,3, 3,4,5, 6,7} );
  }
  SUBCASE("zero-length range") {
    std_e::dynarray<int> x = {};
    std_e::dynarray<int> res(x.size());
    auto res_ptr = unique(x.begin(), x.end(), res.begin());
    CHECK( res_ptr == res.begin() );
    CHECK( res == std_e::dynarray<int>{} );
  }
}

TEST_CASE("remove") {
  std_e::dynarray<int> x = {0,0,1,3,4,0,  1,0,1,5,  6,7};
  std_e::dynarray<int> res(x.size());
  auto res_ptr = res.data();
  res_ptr = remove(x.data()   , x.data()+ 6, res_ptr, 0);
  res_ptr = remove(x.data()+ 6, x.data()+10, res_ptr, 0);
  res_ptr = remove(x.data()+10, x.data()+12, res_ptr, 0);
  auto final_sz = res_ptr - res.data();
  res.resize(final_sz);
  CHECK( res == std_e::dynarray<int>{1,3,4,  1,1,5,  6,7} );
}
