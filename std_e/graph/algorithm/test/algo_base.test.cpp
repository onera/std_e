#if __cplusplus > 201703L
#include "std_e/unit_test/doctest.hpp"

#include "std_e/graph/algorithm/algo_base.hpp"
#include "std_e/graph/test_utils/simple_graph_type.hpp"
//#include "std_e/graph/adjacency_graph/graph.hpp"
//#include "std_e/graph/adjacency_graph/graph_algo.hpp"
//#include "std_e/graph/adjacency_graph/rooted_view.hpp"
#include "std_e/log.hpp"
#include <string>


using namespace std_e;


namespace {

///* Reminder:
//       1
//    /     \
//   2        3
// /  \    /  |  \
//4    7   8  10  11
//         |
//         9
//*/
//
//const auto g = make_graph<int>({
// /* 0*/ { 1, {1,2} },
// /* 1*/ { 2, {3,7} },
// /* 2*/ { 3, {5,7,11} },
// /* 3*/ { 4, {} },
// /* 4*/ { 7, {} },
// /* 5*/ { 8, {6} },
// /* 6*/ { 9, {} },
// /* 7*/ {10, {} },
// /* 8*/ {11, {} },
//});
//
//using adj = io_adjacency<rooted_view<const graph<int>>>;

struct visitor_for_testing_dfs {
  using adj = test::tree;

  auto
  pre(adj& x) {
    s += "pre " + std::to_string(x.value) + "\n";
  }
  auto
  post(adj& x) -> void {
    s += "post " + std::to_string(x.value) + "\n";
  }
  auto
  up(adj& below, adj& above) -> void {
    s += "up " + std::to_string(below.value) + " -> " + std::to_string(above.value) + "\n";
  }
  auto
  down(adj& above, adj& below) -> void {
    s += "down " + std::to_string(above.value) + " -> " + std::to_string(below.value) + "\n";
  }

  std::string s;
};


TEST_CASE("Nested tree preorder depth-first find") {
  std::vector<test::tree> tc = { test::create_tree_for_tests() };

  visitor_for_testing_dfs v;
  auto S = graph_traversal_stack(begin(tc),end(tc));

  depth_first_scan_adjacency_stack(S,v);
  ELOG(v.s);
}


} // anon
#endif // C++20
