#if __cplusplus > 201703L

#include "std_e/unit_test/doctest.hpp"

#include "std_e/graph/adjacency_graph/adjacency_graph_algo.hpp"


using namespace std_e;


TEST_CASE("adjacency_graph") {
  /*
     A <-- B --> C
           |    /
           v   /
           D <-
  */
  auto g = make_io_adjacency_graph<char>({
    /*0*/ {'A', {}   , {}     },
    /*1*/ {'B', {}   , {0,2,3}},
    /*2*/ {'C', {}   , {3}    },
    /*3*/ {'D', {}   , {}     },
  });

  make_bidirectional_from_outward_edges(g);

  auto expected_g = make_io_adjacency_graph<char>({
    /*0*/ {'A', {1}  , {}     },
    /*1*/ {'B', {}   , {0,2,3}},
    /*2*/ {'C', {1}  , {3}    },
    /*3*/ {'D', {1,2}, {}     },
  });

  CHECK( g == expected_g );

  SUBCASE("range type") {
    using graph_type = decltype(g);
    static_assert(std::ranges::random_access_range<graph_type>);
  }
}

#endif // C++20
