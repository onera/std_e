#include "std_e/unit_test/doctest.hpp"
#include "std_e/graph/csr/algorithm.hpp"
#include "std_e/future/dynarray.hpp"


TEST_CASE("renumber_graph") {
  std_e::dynarray<int> idx = {0    ,  3  ,  5,  6  ,  8};
  std_e::dynarray<int>   x = {1,2,3,  3,0,  0,  1,0};
  // Representation of the graph:
  //    0 ______________  1
  //      |\            |
  //      |  \          |
  //      |    \        |
  //      |      \      |
  //      |        \    |
  //      |          \  |
  //    2 |            \| 3
  //

  std_e::dynarray<int> perm = {0,2,3,1};
  std_e::renumber_graph(idx, x, perm);


  // Representation of the permuted graph:
  //    0 ______________  3
  //      |\            |
  //      |  \          |
  //      |    \        |
  //      |      \      |
  //      |        \    |
  //      |          \  |
  //    1 |            \| 2
  //

  CHECK( idx == std_e::dynarray<int>{0    ,  3,  4  ,  6  ,  8} );
  CHECK(   x == std_e::dynarray<int>{3,1,2,  0,  3,0,  2,0}     );
}
