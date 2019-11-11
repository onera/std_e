#include "doctest/doctest.h"
#include "doctest/doctest_utils.hpp"

#include "graph/tree/nested_tree.hpp"
#include "graph/algorithm/algo_nodes.hpp"
#include "graph/test/utils.hpp"
#include <iostream>//TODO DEL
#include <stdexcept>//TODO DEL

#include "graph/adjacency_graph/io_graph.hpp"


using namespace graph;
using namespace std;


TEST_CASE("Nested tree preorder depth-first find") {
  auto t = create_nested_tree_for_tests();

  vector<int> pre_nodes;

  SUBCASE("found in the middle") {
    auto f = [&](int i){  pre_nodes.push_back(i); return i!=8; };
    preorder_depth_first_find(t,f);

    std::vector<int> expected_pre_nodes = {1, 2,4,7, 3,8};

    CHECK( pre_nodes == expected_pre_nodes );
  }

  SUBCASE("stop immediately") {
    auto f = [&](int i){  pre_nodes.push_back(i); return false; };
    preorder_depth_first_find(t,f);

    std::vector<int> expected_pre_nodes = {1};

    CHECK( pre_nodes == expected_pre_nodes );
  }

  SUBCASE("never stop") {
    auto f = [&](int i){  pre_nodes.push_back(i); return true; };
    preorder_depth_first_find(t,f);

    std::vector<int> expected_pre_nodes = {1, 2,4,7, 3,8,9,10,11};

    CHECK( pre_nodes == expected_pre_nodes );
  }
}


struct visitor_for_testing_dfs {
  auto
  pre(int i) {
    s += "pre " + std::to_string(i) + "\n";
  }
  auto
  post(int i) {
    s += "post " + std::to_string(i) + "\n";
  }
  auto
  up(int below, int above) -> long {
    s += "up " + std::to_string(below) + " -> " + std::to_string(above) + "\n";
    return 0;
  }
  auto
  down(int above, int below) -> long {
    s += "down " + std::to_string(above) + " -> " + std::to_string(below) + "\n";
    return 0;
  }

  string s;
};

TEST_CASE("Nested tree prepostorder depth-first scan") {
  auto t = create_nested_tree_for_tests();

  visitor_for_testing_dfs v;
  prepostorder_depth_first_scan(t,v);

  /* Reminder:
         1
      /     \
     2        3
   /  \    /  |  \
  4    7   8  10  11
           |
           9
  */
  string expected_s = 
    "pre 1\n"
    "pre 2\n"
    "pre 4\n"
    "post 4\n"
    "pre 7\n"
    "post 7\n"
    "post 2\n"
    "pre 3\n"
    "pre 8\n"
    "pre 9\n"
    "post 9\n"
    "post 8\n"
    "pre 10\n"
    "post 10\n"
    "pre 11\n"
    "post 11\n"
    "post 3\n"
    "post 1\n";

  CHECK( v.s == expected_s );
}

TEST_CASE("Nested tree preorder depth-first scan") {
  auto t = create_nested_tree_for_tests();

  string s;
  auto f = [&](int i){  s += "pre " + std::to_string(i) + "\n"; };
  preorder_depth_first_scan(t,f);

  string expected_s = 
    "pre 1\n"
    "pre 2\n"
    "pre 4\n"
    "pre 7\n"
    "pre 3\n"
    "pre 8\n"
    "pre 9\n"
    "pre 10\n"
    "pre 11\n";

  CHECK( s == expected_s );
}
TEST_CASE("Nested tree postorder depth-first scan") {
  auto t = create_nested_tree_for_tests();

  string s;
  auto f = [&](int i){  s += "post " + std::to_string(i) + "\n"; };
  postorder_depth_first_scan(t,f);

  string expected_s = 
    "post 4\n"
    "post 7\n"
    "post 2\n"
    "post 9\n"
    "post 8\n"
    "post 10\n"
    "post 11\n"
    "post 3\n"
    "post 1\n";

  CHECK( s == expected_s );
}

TEST_CASE("Nested tree depth-first scan") {
  auto t = create_nested_tree_for_tests();

  visitor_for_testing_dfs v;
  depth_first_scan(t,v);

  /* Reminder:
         1
      /     \
     2        3
   /  \    /  |  \
  4    7   8  10  11
           |
           9
  */
  string expected_s = 
    "pre 1\n"
    "down 1 -> 2\n"
    "pre 2\n"
    "down 2 -> 4\n"
    "pre 4\n"
    "post 4\n"
    "up 4 -> 2\n"
    "down 2 -> 7\n"
    "pre 7\n"
    "post 7\n"
    "up 7 -> 2\n"
    "post 2\n"
    "up 2 -> 1\n"
    "down 1 -> 3\n"
    "pre 3\n"
    "down 3 -> 8\n"
    "pre 8\n"
    "down 8 -> 9\n"
    "pre 9\n"
    "post 9\n"
    "up 9 -> 8\n"
    "post 8\n"
    "up 8 -> 3\n"
    "down 3 -> 10\n"
    "pre 10\n"
    "post 10\n"
    "up 10 -> 3\n"
    "down 3 -> 11\n"
    "pre 11\n"
    "post 11\n"
    "up 11 -> 3\n"
    "post 3\n"
    "up 3 -> 1\n"
    "post 1\n";

  CHECK( v.s == expected_s );
}

struct visitor_for_testing_dfs_prune : visitor_for_testing_dfs {
  auto
  pre(int i) -> bool {
    visitor_for_testing_dfs::pre(i);
    return i!=2;
  }
};
TEST_CASE("Nested tree depth-first prune") {
  auto t = create_nested_tree_for_tests();

  visitor_for_testing_dfs_prune v;
  depth_first_prune(t,v);

  /* Reminder:
         1
      /     \
     2        3
   /  \    /  |  \
  4    7   8  10  11
           |
           9
  */
  string expected_s = 
    "pre 1\n"
    "down 1 -> 2\n"
    "pre 2\n"
    "post 2\n"
    "up 2 -> 1\n"
    "down 1 -> 3\n"
    "pre 3\n"
    "down 3 -> 8\n"
    "pre 8\n"
    "down 8 -> 9\n"
    "pre 9\n"
    "post 9\n"
    "up 9 -> 8\n"
    "post 8\n"
    "up 8 -> 3\n"
    "down 3 -> 10\n"
    "pre 10\n"
    "post 10\n"
    "up 10 -> 3\n"
    "down 3 -> 11\n"
    "pre 11\n"
    "post 11\n"
    "up 11 -> 3\n"
    "post 3\n"
    "up 3 -> 1\n"
    "post 1\n";

  CHECK( v.s == expected_s );
}


struct visitor_for_testing_dfs_prune_adjacencies {
  auto
  pre(const io_adjacency<int>& a) -> bool {
    int i = node(a);
    s += "pre " + std::to_string(i) + "\n";
    return i!=2;
  }
  auto
  post(const io_adjacency<int>& a) -> void {
    int i = node(a);
    s += "post " + std::to_string(i) + "\n";
  }

  std::string s;
};

TEST_CASE("Adj graph depth-first prune") {
  /*
         1               lvl 3
      /  |  \
     |   |    3          lvl 2
     |   | /  |  \
     2\_ | 8  |   \      lvl 1
   /  \ \| |  |    \
   |  |  \ |  |    \
  4    7  \9  10   11    lvl 0
  */
  using IC = connection_indices_container;
  io_index_adjacency_list<int> adj_index_list = {
    /*0*/ { 4,IC{2}    ,IC{}     },
    /*1*/ { 7,IC{2}    ,IC{}     },
    /*2*/ { 2,IC{8}    ,IC{0,1,3}},
    /*3*/ { 9,IC{4,8,2},IC{}     },
    /*4*/ { 8,IC{7}    ,IC{3}    },
    /*5*/ {10,IC{7}    ,IC{}     },
    /*6*/ {11,IC{7}    ,IC{}     },
    /*7*/ { 3,IC{8}    ,IC{4,5,6}},
    /*8*/ { 1,IC{}     ,IC{2,7,3}},
  };
  io_graph<int> io_g(adj_index_list);

  visitor_for_testing_dfs_prune_adjacencies v;
  prepostorder_depth_first_prune_adjacencies(io_g[8],v);

  string expected_s = 
    "pre 1\n"
    "pre 2\n"
    "post 2\n"
    "pre 3\n"
    "pre 8\n"
    "pre 9\n"
    "post 9\n"
    "post 8\n"
    "pre 10\n"
    "post 10\n"
    "pre 11\n"
    "post 11\n"
    "post 3\n"
    "pre 9\n"
    "post 9\n"
    "post 1\n";

  CHECK( v.s == expected_s );
}
