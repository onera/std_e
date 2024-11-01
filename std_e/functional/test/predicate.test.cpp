#include "std_e/unit_test/doctest.hpp"

#include "std_e/functional/predicate.hpp"

using std_e::Predicate;

TEST_CASE("predicate") {
  auto is_even = Predicate([](int i){ return i%2 == 0; });
  auto is_ge_5 = Predicate([](int i){ return i >= 5; }  );
  auto is_8    = Predicate([](int i){ return i == 8; }  );

  std::vector<int> xs = {0,1,2,3,4,5,6,7,8,9};
  std::vector<int> res;

  SUBCASE("ctor and operator()") {
    std::ranges::copy_if(xs, std::back_inserter(res), is_even);
    CHECK( res == std::vector<int>{0,2,4,6,8} );
  }
  SUBCASE("not predicate") {
    std::ranges::copy_if(xs, std::back_inserter(res), !is_ge_5);
    CHECK( res == std::vector<int>{0,1,2,3,4} );
  }
  SUBCASE("and predicate") {
    std::ranges::copy_if(xs, std::back_inserter(res), is_even && is_ge_5 );
    CHECK( res == std::vector<int>{6,8} );
  }
  SUBCASE("or predicate") {
    std::ranges::copy_if(xs, std::back_inserter(res), is_even || is_ge_5 );
    CHECK( res == std::vector<int>{0,2,4,5,6,7,8,9} );
  }
  SUBCASE("complexe predicate") {
    std::ranges::copy_if(xs, std::back_inserter(res), is_even && (!is_ge_5 || is_8));
    CHECK( res == std::vector<int>{0,2,4,8} );
  }
}
