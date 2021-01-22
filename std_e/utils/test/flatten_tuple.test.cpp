#include "std_e/unit_test/doctest.hpp"
#include "std_e/utils/flatten_tuple.hpp"
#include <iostream>
#include <algorithm>
using namespace std_e;


// template<typename T>
// void
// debug_type(T) {
//   std::cout << __PRETTY_FUNCTION__ << std::endl;
// }
// std::cout << " flatten(bare) " << std::endl;
// debug_type(flatten(bare));
// std::cout << " flatten(single) " << std::endl;
// debug_type(flatten(single));
// std::cout << " flatten(nested_simple) " << std::endl;
// debug_type(flatten(nested_simple));

TEST_CASE("flatten tuple static") {
  SUBCASE("simple") {

    constexpr auto bare = 42;

    constexpr auto single = std::tuple{bare};
    constexpr auto nested_simple = std::tuple{single};

    constexpr auto multiple = std::tuple{bare, bare};
    constexpr auto nested_multiple = std::tuple{multiple};

    constexpr auto multiply_nested = std::tuple{multiple, multiple};

    static_assert(flatten(bare) == std::tuple{bare});
    static_assert(flatten(single) == std::tuple{bare});
    static_assert(flatten(nested_simple) == std::tuple{bare});

    static_assert(flatten(multiple) == multiple);
    static_assert(flatten(nested_multiple) == multiple);

    static_assert(flatten(multiply_nested) == std::tuple{bare, bare, bare, bare});
  }

}

