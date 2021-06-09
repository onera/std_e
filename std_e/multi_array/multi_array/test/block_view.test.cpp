#include "std_e/unit_test/doctest.hpp"
#include "std_e/multi_array/multi_array/block_view.hpp"
#include "std_e/multi_array/multi_array.hpp"
#include "std_e/multi_index/multi_index_range.hpp"

using namespace std;
using namespace std_e;

// [Sphinx Doc] block_view {
TEST_CASE("block_view") {
  dyn_multi_array<double,3> ma(4,3,2);
  for (auto [i,j,k] : fortran_multi_index_range(ma.extent())) {
    ma(i,j,k) = 100*i + 10*j + k;
  }
  //                         starting indices   sizes
  //                                    | | |   | | |
  //                                    v v v   v v v
  auto ma_010_322 = make_block_view(ma,{0,1,0},{3,2,2});
  CHECK( ma_010_322.rank() == 3 );
  CHECK( ma_010_322.extent(0) == 3 );
  CHECK( ma_010_322.extent(1) == 2 );
  CHECK( ma_010_322.extent(2) == 2 );

  CHECK( ma_010_322(0,0,0) ==  10. );
  CHECK( ma_010_322(1,0,0) == 110. );
  CHECK( ma_010_322(2,0,0) == 210. );
  CHECK( ma_010_322(0,1,0) ==  20. );
  CHECK( ma_010_322(1,1,0) == 120. );
  CHECK( ma_010_322(2,1,0) == 220. );
  CHECK( ma_010_322(0,0,1) ==  11. );
  CHECK( ma_010_322(1,0,1) == 111. );
  CHECK( ma_010_322(2,0,1) == 211. );
  CHECK( ma_010_322(0,1,1) ==  21. );
  CHECK( ma_010_322(1,1,1) == 121. );
  CHECK( ma_010_322(2,1,1) == 221. );
}
// [Sphinx Doc] block_view }
