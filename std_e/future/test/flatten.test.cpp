#if __cplusplus > 201703L
#include "std_e/unit_test/doctest.hpp"
#include "std_e/future/flatten.hpp"

using namespace std_e;

TEST_CASE("flat_view") {
  std::vector<std::array<int,3>> data = {{3,1,4}, {1,5,9}, {2,6,0}};

  CHECK( flat_view(data) == std::vector{3,1,4, 1,5,9, 2,6,0} );

  flat_view(data)[0] = 10;
  flat_view(data)[4] = 100;
  flat_view(data)[8] = 1000;
  CHECK( flat_view(data) == std::vector{10,1,4, 1,100,9, 2,6,1000} );
}

TEST_CASE("const flat_view") {
  const std::vector<std::array<int,3>> data = {{3,1,4}, {1,5,9}, {2,6,0}};

  CHECK( flat_view(data) == std::vector{3,1,4, 1,5,9, 2,6,0} );
}
#endif // C++>17
