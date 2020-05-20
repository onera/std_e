#include "std_e/unit_test/doctest.hpp"
#include "std_e/multi_index/multi_index.hpp"
#include "std_e/multi_index/fortran_order.hpp"

using namespace std_e;

using MI = multi_index<int,3>;
TEST_CASE("fortran_order_from_dimensions") {
  MI dims = {2,3,4};
  MI i000 = {0,0,0};
  CHECK( fortran_order_from_dimensions(dims, i000) == 0 );

  MI i100 = {1,0,0};
  CHECK( fortran_order_from_dimensions(dims, i100) == 1 );

  MI i123 = {1,2,3};
  CHECK( fortran_order_from_dimensions(dims, i123) == 1 + 2*2 + 3*2*3 );
}

TEST_CASE("fortran_order_from_dimensions with offset") {
  MI dims = {2,3,4};
  MI offsets = {1,1,1};

  MI i_111 = {-1,-1,-1};
  CHECK( fortran_order_from_dimensions(dims, offsets, i_111) == 0 );

  MI i000 = {0,0,0};
  CHECK( fortran_order_from_dimensions(dims, offsets, i000) == 1 + 2 + 2*3 );
}

TEST_CASE("fortran_strides_from_extent") {
  MI dims = {2,3,4};

  MI expected_f_strides_from_extent = {2,2*3,2*3*4};
  CHECK( fortran_strides_from_extent(dims) == expected_f_strides_from_extent );
}

TEST_CASE("fortran_order_from_strides") {
  MI dims = {2,3,4};
  MI strides = fortran_strides_from_extent(dims);

  CHECK( fortran_order_from_strides(strides,MI{0,0,0}) == 0 );
  CHECK( fortran_order_from_strides(strides,MI{1,0,0}) == 1 );
  CHECK( fortran_order_from_strides(strides,MI{1,2,3}) == 1 + 2*2 + 3*2*3 );
}
