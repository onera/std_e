#if __cplusplus > 201703L

#include "std_e/unit_test/doctest.hpp"

#include "std_e/graph/adjacency_graph/simple_graph.hpp"


using namespace std_e;

using adj_list_type = range_of_ranges<int,std::vector>;

TEST_CASE("empty ctor") {
  io_simple_graph<int,double,adj_list_type> x;
  CHECK( x.size() == 0 );
  CHECK( x.nodes().size() == 0 );
  CHECK( x.in_edges().size() == 0 );
  CHECK( x.out_edges().size() == 0 );
  CHECK( x.in_indices().size() == 0 );
  CHECK( x.out_indices().size() == 0 );
}

TEST_CASE("ctor from size") {
  io_simple_graph<int,double,adj_list_type> x(10);
  CHECK( x.size() == 10 );
  CHECK( x.nodes().size() == 10 );
  CHECK( x.in_edges().size() == 10 );
  CHECK( x.out_edges().size() == 10 );
  CHECK( x.in_indices().size() == 10 );
  CHECK( x.out_indices().size() == 10 );
}


#endif // C++20
