#include "doctest/doctest.h"
#include "doctest/doctest_utils.hpp"

#include "graph/adjacency_graph/io_graph.hpp"
#include "graph/test/io_graph.hpp"


using namespace graph;


TEST_CASE("make_bidirectional_from_outward_edges") {
  /*
     A <-- B --> C
           |    /
           v   /
           D <-
  */
  io_index_adjacency_vector<char> idx_adjs = {
    /*0*/ {'A', IC{}    ,IC{}     },
    /*1*/ {'B', IC{}    ,IC{0,2,3}},
    /*2*/ {'C', IC{}    ,IC{3}    },
    /*3*/ {'D', IC{}    ,IC{}     },
  };
  io_graph<char> g(idx_adjs);

  io_index_adjacency_vector<char> expected_bidir_idx_adjs = {
    /*0*/ {'A', IC{1}   ,IC{}     },
    /*1*/ {'B', IC{}    ,IC{0,2,3}},
    /*2*/ {'C', IC{1}   ,IC{3}    },
    /*3*/ {'D', IC{1,2} ,IC{}     },
  };
  io_graph<char> expected_bidir_g(expected_bidir_idx_adjs);

  make_bidirectional_from_outward_edges(g);

  CHECK( g == expected_bidir_g );
}
