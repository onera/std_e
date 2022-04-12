#if __cplusplus > 201703L
#include "std_e/unit_test/doctest.hpp"
#include "std_e/meta/transform_values_to_tuple.hpp"


namespace {

template<int I> struct my_struct {};
constexpr std::array<int,3> my_values = {10,1,4};

TEST_CASE("transform_values_to_tuple") {
  static_assert(
    std::is_same_v<
      std_e::transform_values_to_tuple<my_values,my_struct>,
      std::tuple<my_struct<10>,my_struct<1>,my_struct<4>>
    >
  );
}

} // anonymous
#endif // C++>17
