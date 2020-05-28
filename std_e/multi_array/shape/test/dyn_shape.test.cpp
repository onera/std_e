#include "std_e/unit_test/doctest.hpp"

#include "std_e/multi_array/shape/dyn_shape.hpp"

using namespace std_e;

// [Sphinx Doc] dyn_shape {
TEST_CASE("dyn_shape") {
  dyn_shape<int,3> sh = {{10,20,30},{2,2,2}};

  static_assert( decltype(sh)::rank() == 3 );

  CHECK( sh.rank() == 3 );
  CHECK( sh.size() == 10*20*30 );

  CHECK( sh.extent() == multi_index<int,3>{10,20,30} );
  CHECK( sh.offset() == multi_index<int,3>{ 2, 2, 2} );

  CHECK( sh.extent(0) == 10 );
  CHECK( sh.extent(1) == 20 );
  CHECK( sh.extent(2) == 30 );

  CHECK( sh.offset(0) == 2 );
  CHECK( sh.offset(1) == 2 );
  CHECK( sh.offset(2) == 2 );
}
// [Sphinx Doc] dyn_shape }
