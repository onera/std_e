#if __cplusplus > 201703L
#include "std_e/unit_test/doctest.hpp"

#include "std_e/future/ranges/subrange.hpp"
#include <ranges>
#include <vector>


TEST_CASE("transform") {
  std::vector<int> x = {0,1,2,3};
  auto y = std_e::ranges::subrange(begin(x)+1,end(x)-1);

  CHECK( y[0] == 1 );
  CHECK( y[1] == 2 );
}
#endif // C++>17
