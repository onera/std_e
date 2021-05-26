#include "std_e/unit_test/doctest.hpp"

#include "std_e/graph/test_utils/io_graph.hpp"

#include "std_e/graph/adjacency_graph/graph_rearranging.hpp"
#include "std_e/graph/build/io_graph.hpp"

#include <algorithm>

using namespace std_e;


TEST_CASE("bidirectional_graph_from_outward_edges") {
  /*
     A <-- B --> C
           |    /
           v   /
           D <-
  */
  auto g = make_io_adjacency_graph<char>({
    /*0*/ {'A', {}, {}     },
    /*1*/ {'B', {}, {0,2,3}},
    /*2*/ {'C', {}, {3}    },
    /*3*/ {'D', {}, {}     },
  });

  graph_rearranging_view reordered_g(g);

  std::reverse(begin(reordered_g),end(reordered_g));
  // old pos: {0,1,2,3}
  // new pos: {3,2,1,0}

  //io_adjacency_graph<char> permuted_g = bidirectional_graph_from_outward_edges(reordered_g);
  auto permuted_g = bidirectional_graph_from_outward_edges(reordered_g);

  // expected permuted graph:
  //  - permute node order
  //  - permute outward pointers accordingly
  //  - overwrite inward pointers by using outward pointers
  auto expected_permuted_g = make_io_adjacency_graph<char>({
    /*3*/ {'D', {1,2}, {}     },
    /*2*/ {'C', {2}  , {0}    },
    /*1*/ {'B', {}   , {3,1,0}},
    /*0*/ {'A', {2}  , {}     },
  });

  CHECK( permuted_g == expected_permuted_g );
}


TEST_CASE("Tree reverse_levels") {
  io_adjacency_graph<int> io_g = create_io_graph_for_tests();

  graph_rearranging_view reordered_g(io_g);

  std::reverse(begin(reordered_g),end(reordered_g));
  // old pos: {0,1,2,3,4,5,6,7,8}
  // new pos: {8,7,6,5,4,3,2,1,0}

  io_adjacency_graph<int> permuted_g = bidirectional_graph_from_outward_edges(reordered_g);

  // expected permuted graph:
  //  - permute node order
  //  - permute outward pointers accordingly
  //  - overwrite inward pointers by using outward pointers
  auto expected_permuted_g = make_io_adjacency_graph<int>({
    /*8*/ { 1, {}     , {6,1,5} },
    /*7*/ { 3, {0}    , {4,3,2} },
    /*6*/ {11, {1}    , {}      },
    /*5*/ {10, {1}    , {}      },
    /*4*/ { 8, {1}    , {5}     },
    /*3*/ { 9, {0,4,6}, {}      },
    /*2*/ { 2, {0}    , {8,7,5} },
    /*1*/ { 7, {6}    , {}      },
    /*0*/ { 4, {6}    , {}      },
  });

  CHECK( permuted_g == expected_permuted_g );
}
