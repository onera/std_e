#include "std_e/unit_test/doctest.hpp"

#include "std_e/multi_array/shape/fixed_shape.hpp"

using namespace std_e;

// [Sphinx Doc] fixed_shape {
TEST_CASE("fixed shape") {
  using fixed_shape_type = fixed_shape<5,4,3,2>;

  static_assert( fixed_shape_type::rank() == 4 );
  static_assert( fixed_shape_type::size() == 5*4*3*2 );

  #if __cplusplus > 201703L // clang <=10 problem
  static_assert( fixed_shape_type::extent() == multi_index<int,4>{5,4,3,2} );
  static_assert( fixed_shape_type::offset() == zero_multi_index<int,4> );
  #endif
 

  SUBCASE("also works for a specific axis") {
    static_assert( fixed_shape_type::extent(0) == 5 );
    static_assert( fixed_shape_type::offset(0) == 0 );
  }

  SUBCASE("also works with member function syntax") {
    fixed_shape_type sh;

    CHECK( sh.rank() == 4 );
    CHECK( sh.size() == 5*4*3*2 );

    CHECK( sh.extent() == multi_index<int,4>{5,4,3,2} );
    CHECK( sh.offset() == zero_multi_index<int,4> );

    CHECK( sh.extent(0) == 5 );
    CHECK( sh.offset(0) == 0 );
  }
}
// [Sphinx Doc] fixed_shape }
