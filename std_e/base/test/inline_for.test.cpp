#include "std_e/unit_test/doctest.hpp"
#include <vector>
#include "std_e/base/inline_for.hpp"


namespace {
  struct inline_for_callable {
    template<int i> FORCE_INLINE static constexpr auto
    call(std::vector<int>& v) -> void {
      v[i] = 42 + i;
    }
  };
}

TEST_CASE("inline_for") {
  std::vector<int> v(5);

  std_e::inline_for<5,inline_for_callable>(v);

  std::vector<int> expected_v = {42,43,44,45,46};
  CHECK( v == expected_v );
}
