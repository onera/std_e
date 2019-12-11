#include "doctest/doctest.h"
#include "doctest/doctest_utils.hpp"
#include "graph/test/doctest_utils.hpp"

#include "graph/test/io_graph.hpp"

#include "graph/adjacency_graph/io_graph_rearranging.hpp"
#include "graph/adjacency_graph/build_adjacency_graph.hpp"

#include <algorithm>

using namespace graph;


TEST_CASE("Tree reverse_levels") {
  io_graph<int> io_g = create_io_graph_for_tests();


  io_graph_rearranging_view<int> reordered_io_graph(io_g);

  std::reverse(begin(reordered_io_graph),end(reordered_io_graph));
  // old pos: {0,1,2,3,4,5,6,7,8}
  // new pos: {8,7,6,5,4,3,2,1,0}

  io_graph<int> permuted_io_graph = bidirectional_graph_from_outward_edges(reordered_io_graph);

  // expected permuted graph:
  //  - permute element order
  //  - permute inward and outward node indices
  io_index_adjacency_vector<int> expected_perm_idx_adjs = {
    /*8*/ { 1,IC{}     ,IC{6,1,5}},
    /*7*/ { 3,IC{0}    ,IC{4,3,2}},
    /*6*/ {11,IC{1}    ,IC{}     },
    /*5*/ {10,IC{1}    ,IC{}     },
    /*4*/ { 8,IC{1}    ,IC{5}    },
    /*3*/ { 9,IC{0,4,6},IC{}     },
    /*2*/ { 2,IC{0}    ,IC{8,7,5}},
    /*1*/ { 7,IC{6}    ,IC{}     },
    /*0*/ { 4,IC{6}    ,IC{}     },
  };
  io_graph<int> expected_permuted_io_graph(expected_perm_idx_adjs);

  CHECK( permuted_io_graph == expected_permuted_io_graph );
}
