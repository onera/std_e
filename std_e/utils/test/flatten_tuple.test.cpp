#include "std_e/unit_test/doctest.hpp"
#include "std_e/utils/flatten_tuple.hpp"

#include <algorithm>

TEST_CASE("flatten tuple static ") {
  SUBCASE(" simple ") {

    constexpr auto bare = 42;

    constexpr auto single = std::tuple{bare};
    constexpr auto nested_simple = std::tuple{single};

    constexpr auto multiple = std::tuple{bare, bare};
    constexpr auto nested_multiple = std::tuple{multiple};

    constexpr auto multiply_nested = std::tuple{multiple, multiple};

    static_assert(flatten(bare) == bare);
    static_assert(flatten(single) == bare);
    static_assert(flatten(nested_simple) == bare);

    static_assert(flatten(multiple) == multiple);
    static_assert(flatten(nested_multiple) == multiple);

    static_assert(flatten(multiply_nested) == std::tuple{bare, bare, bare, bare});
  }

}

