#include "std_e/unit_test/doctest.hpp"

#include "std_e/graph/nested_tree/tree.hpp"
#include "std_e/graph/algorithm/algo_adjacencies.hpp"

using namespace std_e;
using std::vector;

auto create_tree_for_tests() {
  return
    Tree<int>(1, std::vector<Tree<int>>{
      Tree<int>(2, std::vector<Tree<int>>{
        Tree<int>(4),
        Tree<int>(7)
      }),
      Tree<int>(3, std::vector<Tree<int>>{
        Tree<int>(8 , std::vector<Tree<int>>{
          Tree<int>(9)
        }),
        Tree<int>(10),
        Tree<int>(11)
      })
    });
}

TEST_CASE("Tree depth-first find") {

  auto t = create_tree_for_tests();
    /* Reminder:
           1
        /     \
       2        3
     /  \    /  |  \
    4    7   8  10  11
             |
             9
    */

  vector<int> pre_nodes;

  SUBCASE("found in the middle") {
    auto f = [&](const auto& adj){  pre_nodes.push_back(node(adj)); return node(adj)==8; };
    auto found_sub_tree = preorder_depth_first_find_adjacencies(t,f);

    std::vector<int> expected_pre_nodes = {1, 2,4,7, 3,8};

    CHECK( pre_nodes == expected_pre_nodes );

    SUBCASE("return value") {
      int& found_node = node(*found_sub_tree);
      CHECK( found_node == 8 );
      found_node = 12;
      CHECK( t.children[1].children[0].node == 12 );
    }
  }
}
