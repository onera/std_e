#if __cplusplus > 201703L
#include "std_e/unit_test/doctest.hpp"

#include "std_e/future/ranges.hpp"
#include "std_e/future/ranges/transform.hpp"
#include <ranges>


TEST_CASE("transform") {
  std::vector x = {0,1,2,3};
  auto v = x | std_e::ranges::views::transform([=](int i){ return i+10; });
  
  int cnt = 10;
  for (auto i : v) {
    CHECK( i == cnt );
    ++cnt;
  }
}
#endif // C++>17
