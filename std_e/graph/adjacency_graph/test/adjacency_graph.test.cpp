#if __cplusplus > 201703L

#include "std_e/unit_test/doctest.hpp"

#include "std_e/graph/adjacency_graph/adjacency_graph_algo.hpp"


using namespace std_e;


TEST_CASE("make_bidirectional_from_outward_edges") {
  /*
     A <-- B --> C
           |    /
           v   /
           D <-
  */
  auto g = make_io_adjacency_graph<char>({
    /*0*/ {'A', {}    ,{}     },
    /*1*/ {'B', {}    ,{0,2,3}},
    /*2*/ {'C', {}    ,{3}    },
    /*3*/ {'D', {}    ,{}     },
  });

  make_bidirectional_from_outward_edges(g);

  auto expected_g = make_io_adjacency_graph( io_index_adjacency_vector<char>{
    /*0*/ {'A', {1}   ,{}     },
    /*1*/ {'B', {}    ,{0,2,3}},
    /*2*/ {'C', {1}   ,{3}    },
    /*3*/ {'D', {1,2} ,{}     },
  });

  CHECK( g == expected_g );
}

#endif // C++20
