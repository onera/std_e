#if __cplusplus > 201703L

#include "std_e/unit_test/doctest.hpp"

#include "std_e/graph/adjacency_graph/graph_algo.hpp"


using namespace std_e;


TEST_CASE("adjacency_range") {
  /*
     A <-- B --> C
           |    /
           v   /
           D <-
  */
  auto g = make_io_graph<char>({
    /*0*/ {'A', {}, {}     },
    /*1*/ {'B', {}, {0,2,3}},
    /*2*/ {'C', {}, {3}    },
    /*3*/ {'D', {}, {}     },
  });

  auto  in_rng = g[1]. in_adjacencies();
  auto out_rng = g[1].out_adjacencies();

  SUBCASE("range type") {
    using  in_adj_range_type = decltype( in_rng);
    using out_adj_range_type = decltype(out_rng);
    static_assert(std::ranges::random_access_range< in_adj_range_type>);
    static_assert(std::ranges::random_access_range<out_adj_range_type>);
  }

  SUBCASE("range operations") {
    CHECK( in_rng.size() == 0 );
    CHECK( out_rng.size() == 3 );

    CHECK( *out_rng.begin() == g[0] );
    CHECK( out_rng.end() == out_rng.begin()+3 );

    CHECK( out_rng[0] == g[0] );
    CHECK( out_rng[1] == g[2] );
    CHECK( out_rng[2] == g[3] );
  }
}

#endif // C++20
