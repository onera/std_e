#include "doctest/doctest.h"
#include "doctest/doctest_utils.hpp"
#include "graph/test/doctest_utils.hpp"

#include "graph/tree/nested_tree.hpp"
#include "graph/test/utils.hpp"

#include "graph/adjacency_graph/io_graph_rearranging.hpp"
#include "graph/adjacency_graph/build_adjacency_graph.hpp"

#include <algorithm>

using namespace graph;


TEST_CASE("Tree reverse_levels") {
  /*
         1               lvl 3
      /  |  \
     |   |    3          lvl 2
     |   | /  |  \
     2\_ | 8  |   \      lvl 1
   /  \ \| |  |    \
   |  |  \ |  |    \
  4    7  \9  10   11    lvl 0
  */
  using IC = connection_indices_container;
  io_index_adjacency_list<int> adj_index_list = {
    /*0*/ { 4,IC{2}    ,IC{}     },
    /*1*/ { 7,IC{2}    ,IC{}     },
    /*2*/ { 2,IC{8}    ,IC{0,1,3}},
    /*3*/ { 9,IC{4,8,2},IC{}     },
    /*4*/ { 8,IC{7}    ,IC{3}    },
    /*5*/ {10,IC{7}    ,IC{}     },
    /*6*/ {11,IC{7}    ,IC{}     },
    /*7*/ { 3,IC{8}    ,IC{4,5,6}},
    /*8*/ { 1,IC{}     ,IC{2,7,3}},
  };
  io_graph<int> io_g(adj_index_list);


  io_graph_rearranging_view<int> reordered_io_graph(io_g);

  std::reverse(begin(reordered_io_graph),end(reordered_io_graph));
  // old pos: {0,1,2,3,4,5,6,7,8}
  // new pos: {8,7,6,5,4,3,2,1,0}

  io_graph<int> permuted_io_graph = bidirectional_graph_from_outward_edges(reordered_io_graph);

  // expected permuted graph:
  //  - permute element order
  //  - permute inward and outward node indices
  io_index_adjacency_list<int> expected_perm_adj_index_list = {
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
  io_graph<int> expected_permuted_io_graph(expected_perm_adj_index_list);

  CHECK( permuted_io_graph == expected_permuted_io_graph );
}
