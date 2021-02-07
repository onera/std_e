#include "std_e/unit_test/doctest.hpp"

#include "std_e/graph/tree/nested_tree.hpp"
#include "std_e/graph/algorithm/algo_nodes.hpp"
#include "std_e/graph/test/nested_tree.hpp"

#include "std_e/graph/algorithm/zip.hpp"


TEST_CASE("zip_graphs") {
  auto t0 = graph::create_nested_tree_for_tests();
  /* t0 =
         1
      /     \
     2        3
   /  \    /  |  \
  4    7   8  10  11
           |
           9
  */

  SUBCASE("same graph shape") {
    auto t1 = graph::create_nested_tree_for_tests();
    graph::preorder_depth_first_scan(t1,[](auto& node){ node *= 3; });
    /* t1 = 3*t0, so t1 =
           3
        /     \
       6        9
     /  \    /  |  \
    12   21  24 30  33
             |
             27
    */

    auto t = std_e::zip_graphs(t0,t1);

    std::string s;
    auto f = [&](const auto& x){  s += std::to_string(graph::node(x.first)) + "," + std::to_string(graph::node(x.second)) + "\n"; };
    preorder_depth_first_scan_adjacencies(t,f);

    std::string expected_s =
      "1,3\n"
      "2,6\n"
      "4,12\n"
      "7,21\n"
      "3,9\n"
      "8,24\n"
      "9,27\n"
      "10,30\n"
      "11,33\n";

    CHECK( s == expected_s );
  }

  SUBCASE("different graph shape") {
    /* t10 =
             10
          /     \
         20       30
       /  \    /
      40   70  80
    */
    auto t40 = graph::tree<int>{40};
    auto t70 = graph::tree<int>{70};
    auto t20 = graph::create_tree(20,t40,t70);
    auto t80 = graph::tree<int> {80};
    auto t30 = graph::create_tree(30,t80);
    auto t10 = graph::create_tree(10,t20,t30);

    auto t = std_e::zip_graphs(t0,t10);

    std::string s;
    auto f = [&](const auto& x){
      s += std::to_string(graph::node(x.first)) + "," + std::to_string(graph::node(x.second)) + "\n";
      return nb_children(x.first) != nb_children(x.second);
    };
    preorder_depth_first_prune_adjacencies(t,f);

    std::string expected_s =
      "1,10\n"
      "2,20\n"
      "4,40\n"
      "7,70\n"
      "3,30\n";

    CHECK( s == expected_s );
  }
}
