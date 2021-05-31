#if __cplusplus > 201703L

#include "std_e/unit_test/doctest.hpp"

#include "std_e/graph/adjacency_graph/adjacency_graph_algo.hpp"


using namespace std_e;


TEST_CASE("adjacency_node_iterator") {
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

  auto it = g.begin();

  SUBCASE("iterator type") {
    using adj_node_it_type = decltype(it);
    static_assert(std::random_access_iterator<adj_node_it_type>);
  }

  SUBCASE("operator*, operator[]") {
    CHECK( *it == g[0] );

    CHECK( it[0] == g[0] );
    CHECK( it[1] == g[1] );
    CHECK( it[2] == g[2] );
    CHECK( it[3] == g[3] );
  }
  SUBCASE("++it") {
    CHECK( *(++it) == g[1] );
  }
  SUBCASE("--it") {
    ++it;
    CHECK( *(--it) == g[0] );
  }
  SUBCASE("it++") {
    CHECK( *(it++) == g[0] );
    CHECK( *it == g[1] );
  }
  SUBCASE("it--") {
    ++it;
    CHECK( *(it--) == g[1] );
    CHECK( *it == g[0] );
  }
  SUBCASE("it+=i") {
    CHECK( *(it+=2) == g[2] );
  }
  SUBCASE("it-=i") {
    CHECK( *(it-=(-2)) == g[2] );
  }
  SUBCASE("it+i, i+it") {
    CHECK( *(it+2) == g[2] );
    CHECK( *(2+it) == g[2] );
  }
  SUBCASE("it-i") {
    CHECK( *(it-(-2)) == g[2] );
  }
  SUBCASE("it-it") {
    auto it2 = it+2;
    CHECK( it2-it == 2 );
  }

  SUBCASE("copy ctor") {
    auto it0 = it;
    CHECK( *it0 == g[0] );
  }
  SUBCASE("comparisons") {
    auto it0 = it;
    CHECK( it0 == it );
    auto it1 = it+1;
    CHECK( it1 != it );
    CHECK( it  < it1 );
  }
}

#endif // C++20
