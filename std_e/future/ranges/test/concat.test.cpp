#if __cplusplus > 201703L
#include "std_e/unit_test/doctest.hpp"

#include "std_e/future/ranges/concat.hpp"
#include <vector>
#include <deque>


TEST_CASE("concat") {
  SUBCASE("with lvalues|lvalues") {
    std::vector<int> v = {0,1,2};
    std::deque<int> d = {3,4};

    auto cat_view = std_e::views::concat(v,d);
    auto cat_vector = cat_view | std_e::to_vector();
    CHECK( cat_vector == std::vector{0,1,2,3,4} );
  }

  SUBCASE("with lvalues|rvalues") {
    std::vector<int> v = {0,1,2};
    std::deque<int> d = {3,4};

    auto cat_vector = std_e::views::concat(v,d) | std_e::to_vector();
    CHECK( cat_vector == std::vector{0,1,2,3,4} );
  }

  SUBCASE("with rvalues|rvalues") {
    auto cat_vector = std_e::views::concat(std::vector{0,1,2},std::deque{3,4}) | std_e::to_vector();
    CHECK( cat_vector == std::vector{0,1,2,3,4} );
  }

  SUBCASE("with nested concat_view") {
    std::vector<int> v = {0,1,2};
    std::deque<int> d = {3,4};
    auto nested_cat_view = std_e::views::concat(v,d);

    std::vector<int> w = {5,6,7};
    auto cat_vector = std_e::views::concat(nested_cat_view,w) | std_e::to_vector();
    CHECK( cat_vector == std::vector{0,1,2,3,4,5,6,7} );
  }
}
#endif // C++>17
