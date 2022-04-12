#if __cplusplus > 201703L
#include "std_e/unit_test/doctest.hpp"

#include "std_e/future/ranges/repeat.hpp"
#include "std_e/future/ranges.hpp"
#include <ranges>


TEST_CASE("repeat") {
  static_assert(std::ranges::forward_range < std_e::views::repeat_view<int> >);
  static_assert(std::ranges::viewable_range< std_e::views::repeat_view<int> >);

  auto r = std_e::ranges::repeat(97, 4) | std_e::to_vector() ;
  CHECK( r == std::vector{97,97,97,97} );
}
#endif // C++>17
