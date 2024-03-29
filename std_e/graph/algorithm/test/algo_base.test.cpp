#if __cplusplus > 201703L
#include "std_e/unit_test/doctest.hpp"

#include "std_e/graph/algorithm/algo_base.hpp"
#include "std_e/graph/test_utils/graph.hpp"
#include <string>


using namespace std_e;


namespace {


struct visitor_for_testing_depth_first_scan {
  auto
  pre(auto&& x) -> void {
    s += "[pre ] " + std::to_string(node(x)) + "\n";
  }
  auto
  post(auto&& x) -> void {
    s += "[post] " + std::to_string(node(x)) + "\n";
  }
  auto
  up(auto&& below, auto&& above) -> void {
    s += "[up  ] " + std::to_string(node(below)) + " -> " + std::to_string(node(above)) + "\n";
  }
  auto
  down(auto&& above, auto&& below) -> void {
    s += "[down] " + std::to_string(node(above)) + " -> " + std::to_string(node(below)) + "\n";
  }

  std::string s;
};
TEST_CASE("depth_first_scan_adjacency_stack") {
  /* Reminder:
         1               lvl 3
      /  |  \
     |   |    3          lvl 2
     |   | /  |  \
     2\_ | 8  |   \      lvl 1
   /  \ \| |  |    \
   |  |  \ |  |    \
  4    7  \9  10   11    lvl 0
  */
  auto g = create_rooted_graph_for_tests();
  auto S = graph_traversal_stack(first_root(g),last_root(g));

  visitor_for_testing_depth_first_scan v;
  depth_first_scan_adjacency_stack(S,v);

  std::string expected_s =
    "[pre ] 1\n"
    "[down] 1 -> 2\n"
    "[pre ] 2\n"
    "[down] 2 -> 4\n"
    "[pre ] 4\n"
    "[post] 4\n"
    "[up  ] 4 -> 2\n"
    "[down] 2 -> 7\n"
    "[pre ] 7\n"
    "[post] 7\n"
    "[up  ] 7 -> 2\n"
    "[down] 2 -> 9\n"
    "[pre ] 9\n"
    "[post] 9\n"
    "[up  ] 9 -> 2\n"
    "[post] 2\n"
    "[up  ] 2 -> 1\n"
    "[down] 1 -> 3\n"
    "[pre ] 3\n"
    "[down] 3 -> 8\n"
    "[pre ] 8\n"
    "[down] 8 -> 9\n"
    "[pre ] 9\n"
    "[post] 9\n"
    "[up  ] 9 -> 8\n"
    "[post] 8\n"
    "[up  ] 8 -> 3\n"
    "[down] 3 -> 10\n"
    "[pre ] 10\n"
    "[post] 10\n"
    "[up  ] 10 -> 3\n"
    "[down] 3 -> 11\n"
    "[pre ] 11\n"
    "[post] 11\n"
    "[up  ] 11 -> 3\n"
    "[post] 3\n"
    "[up  ] 3 -> 1\n"
    "[down] 1 -> 9\n"
    "[pre ] 9\n"
    "[post] 9\n"
    "[up  ] 9 -> 1\n"
    "[post] 1\n";

  CHECK( v.s == expected_s );
}


struct visitor_for_testing_depth_first_find : visitor_for_testing_depth_first_scan {
  auto
  pre(auto&& x) -> bool {
    visitor_for_testing_depth_first_scan::pre(x);
    return node(x) == 3;
  }
};
TEST_CASE("depth_first_find_adjacency_stack") {
  /* Reminder:
         1               lvl 3
      /  |  \
     |   |    3          lvl 2
     |   | /  |  \
     2\_ | 8  |   \      lvl 1
   /  \ \| |  |    \
   |  |  \ |  |    \
  4    7  \9  10   11    lvl 0
  */
  auto g = create_rooted_graph_for_tests();
  auto S = graph_traversal_stack(first_root(g),last_root(g));

  visitor_for_testing_depth_first_find v;
  auto found = depth_first_find_adjacency_stack(S,v);

  std::string expected_s =
    "[pre ] 1\n"
    "[down] 1 -> 2\n"
    "[pre ] 2\n"
    "[down] 2 -> 4\n"
    "[pre ] 4\n"
    "[post] 4\n"
    "[up  ] 4 -> 2\n"
    "[down] 2 -> 7\n"
    "[pre ] 7\n"
    "[post] 7\n"
    "[up  ] 7 -> 2\n"
    "[down] 2 -> 9\n"
    "[pre ] 9\n"
    "[post] 9\n"
    "[up  ] 9 -> 2\n"
    "[post] 2\n"
    "[up  ] 2 -> 1\n"
    "[down] 1 -> 3\n"
    "[pre ] 3\n"
    "[post] 3\n"
    "[up  ] 3 -> 1\n"
    "[post] 1\n";

  CHECK( v.s == expected_s );
  CHECK( node(*found) == 3 );
}


struct visitor_for_testing_depth_first_prune : visitor_for_testing_depth_first_scan {
  auto
  pre(auto&& x) -> bool {
    visitor_for_testing_depth_first_scan::pre(x);
    return node(x) == 2;
  }
};
TEST_CASE("depth_first_prune_adjacency_stack") {
  /* Reminder:
         1               lvl 3
      /  |  \
     |   |    3          lvl 2
     |   | /  |  \
     2\_ | 8  |   \      lvl 1
   /  \ \| |  |    \
   |  |  \ |  |    \
  4    7  \9  10   11    lvl 0
  */
  auto g = create_rooted_graph_for_tests();
  auto S = graph_traversal_stack(first_root(g),last_root(g));

  visitor_for_testing_depth_first_prune v;
  depth_first_prune_adjacency_stack(S,v);

  std::string expected_s =
    "[pre ] 1\n"
    "[down] 1 -> 2\n"
    "[pre ] 2\n"
    "[post] 2\n"
    "[up  ] 2 -> 1\n"
    "[down] 1 -> 3\n"
    "[pre ] 3\n"
    "[down] 3 -> 8\n"
    "[pre ] 8\n"
    "[down] 8 -> 9\n"
    "[pre ] 9\n"
    "[post] 9\n"
    "[up  ] 9 -> 8\n"
    "[post] 8\n"
    "[up  ] 8 -> 3\n"
    "[down] 3 -> 10\n"
    "[pre ] 10\n"
    "[post] 10\n"
    "[up  ] 10 -> 3\n"
    "[down] 3 -> 11\n"
    "[pre ] 11\n"
    "[post] 11\n"
    "[up  ] 11 -> 3\n"
    "[post] 3\n"
    "[up  ] 3 -> 1\n"
    "[down] 1 -> 9\n"
    "[pre ] 9\n"
    "[post] 9\n"
    "[up  ] 9 -> 1\n"
    "[post] 1\n";

  CHECK( v.s == expected_s );
}


struct visitor_for_testing_dfs : visitor_for_testing_depth_first_scan {
  auto
  pre(auto&& x) -> step {
    visitor_for_testing_depth_first_scan::pre(x);
    if (node(x) == 8) return step::out;
    if (node(x) == 2) return step::over;
    else return step::into;
  }
};
TEST_CASE("depth_first_search_adjacency_stack") {
  /* Reminder:
         1               lvl 3
      /  |  \
     |   |    3          lvl 2
     |   | /  |  \
     2\_ | 8  |   \      lvl 1
   /  \ \| |  |    \
   |  |  \ |  |    \
  4    7  \9  10   11    lvl 0
  */
  auto g = create_rooted_graph_for_tests();
  auto S = graph_traversal_stack(first_root(g),last_root(g));

  visitor_for_testing_dfs v;
  depth_first_search_adjacency_stack(S,v);

  std::string expected_s =
    "[pre ] 1\n"
    "[down] 1 -> 2\n"
    "[pre ] 2\n"
    "[post] 2\n"
    "[up  ] 2 -> 1\n"
    "[down] 1 -> 3\n"
    "[pre ] 3\n"
    "[down] 3 -> 8\n"
    "[pre ] 8\n"
    "[post] 8\n"
    "[up  ] 8 -> 3\n"
    "[post] 3\n"
    "[up  ] 3 -> 1\n"
    "[post] 1\n";

  CHECK( v.s == expected_s );
}
struct modifying_visitor_for_testing_dfs : visitor_for_testing_depth_first_scan {
  auto
  pre(auto&& x) -> step {
    step s = step::into;
    if (node(x) == 8) s = step::out;
    if (node(x) == 2) s = step::over;

    node(x) += 100;
    return s;
  }
  auto
  post(auto&& x) -> void {
    node(x) += 10000;
  }
};
TEST_CASE("depth_first_search_adjacency_stack_inplace_modif") {
  /* Reminder:
         1               lvl 3
      /  |  \
     |   |    3          lvl 2
     |   | /  |  \
     2\_ | 8  |   \      lvl 1
   /  \ \| |  |    \
   |  |  \ |  |    \
  4    7  \9  10   11    lvl 0
  */

  auto g = create_rooted_graph_for_tests();
  auto S = graph_traversal_stack(first_root(g),last_root(g));

  modifying_visitor_for_testing_dfs v;
  depth_first_search_adjacency_stack(S,v);

  CHECK( g.nodes() == std::vector{4,7,10102,9,10108,10,11,10103,10101} );
}


} // anon
#endif // C++20
