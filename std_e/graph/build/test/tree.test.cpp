#include "std_e/unit_test/doctest.hpp"

#include "std_e/graph/tree/nested_tree.hpp"
#include "std_e/graph/test_utils/io_graph.hpp"
#include "std_e/graph/build/tree.hpp"

#include "std_e/graph/test_utils/nested_tree.hpp"
#include "std_e/graph/adjacency_graph/rooted_view.hpp"
#include <numeric>
#include <iostream>


using namespace graph;
using namespace std_e;

// create_tree_from_nodes - same tree structure {
struct builder_0_for_test {
  using tree_type = tree<int>;
  auto
  build(int node) -> tree<int> {
    return tree<int>(node);
  }
};

auto
tree_built_0_for_test() -> tree<int> {
  /* Tree equivalent to the graph of create_io_graph_for_tests()
       ____1____
      /    |    \
     2     3     9
    /|\   /| \
   4 7 9 8 10 11
         |
         9
  */
  auto t4 = tree<int>{4};
  auto t7 = tree<int>{7};
  auto t9 = tree<int> {9};
  auto t2 = create_tree(2,t4,t7,t9);

  auto t8 = create_tree(8,t9);

  auto t10 = tree<int>{10};
  auto t11 = tree<int>{11};
  auto t3 = create_tree(3,t8,t10,t11);

  auto t1 = create_tree(1,t2,t3,t9);

  return t1;
}

TEST_CASE("create_tree_from_nodes - same tree structure") {
  io_adjacency_graph<int> g = create_io_graph_for_tests();
  auto root = rooted_view(g,8);

  auto t = create_tree_from_nodes(root,builder_0_for_test{});

  CHECK( t == tree_built_0_for_test() );
}
// create_tree_from_nodes - same tree structure }


// create_tree_from_nodes - more complex tree structure {
struct builder_1_for_test {
  using tree_type = tree<double>;
  auto
  build(int node) -> tree<double> {
    auto tl = tree<double>(node*0.5);
    auto tr = tree<double>(node*1.5);
    return create_tree(node,tl,tr);
  }
};

auto
tree_init_1_for_test() -> tree<int> {
  /*
        3
      /   \
      8   10
      |
      9
  */
  auto t9 = tree<int> {9};
  auto t8 = create_tree(8,t9);

  auto t10 = tree<int>{10};
  auto t3 = create_tree(3,t8,t10);

  return t3;
}
auto
tree_built_1_for_test() -> tree<double> {
/*
Reminder: t_init
  3
    8
      9
    10

Result tree:
  3
    1.5
    4.5
    8
      4.
      12.
      9
        4.5
        13.5
    10
      5.
      15.
  */
  auto t1_5 = tree<double>{1.5};
  auto t4_5 = tree<double>{4.5};

  auto t4 = tree<double>{4.};
  auto t12 = tree<double>{12.};

  auto t13_5 = tree<double>{13.5};

  auto t5 = tree<double>{5.};
  auto t15 = tree<double>{15.};


  auto t9 = create_tree(9,t4_5,t13_5);
  auto t8 = create_tree(8,t4,t12,t9);

  auto t10 = create_tree(10,t5,t15);

  auto t3 = create_tree(3,t1_5,t4_5,t8,t10);

  return t3;
}


TEST_CASE("create_tree_from_nodes - more complex tree structure") {
  tree<int> t_init = tree_init_1_for_test();

  auto t = create_tree_from_nodes(t_init,builder_1_for_test{});

  CHECK( t == tree_built_1_for_test() );
}
// create_tree_from_nodes - more complex tree structure }


// create_tree_from_adjacencies {
struct builder_2_for_test {
  using tree_type = tree<int>;
  using adjacency_type = const_tree_view<int>;
  auto
  build(const adjacency_type& adj) -> tree<int> {
    auto sum = node(adj);
    auto cs = children(adj);
    sum = std::accumulate(begin(cs),end(cs),sum,[](int i,auto t){ return i+node(t); });
    return tree<int>(sum);
  }
};
auto
tree_built_2_for_test() -> tree<int> {
  /*
        3+8+10
      /   \
      8+9 10
      |
      9
  */
  auto t9 = tree<int> {9};
  auto t8 = create_tree(8+9,t9);

  auto t10 = tree<int>{10};
  auto t3 = create_tree(3+8+10,t8,t10);

  return t3;
}
TEST_CASE("create_tree_from_adjacencies") {
  tree<int> t_init = tree_init_1_for_test();

  auto t = create_tree_from_adjacencies(t_init,builder_2_for_test{});

  CHECK( t == tree_built_2_for_test() );
}
// create_tree_from_adjacencies }


// create_pruned_tree_from_nodes {
struct builder_3_for_test {
  using tree_type = tree<int>;
  auto
  should_go_down(int node) -> bool {
    return node!=3;
  }
  auto
  build(int node) -> tree<int> {
    return tree<int>(node*2);
  }
};

auto
tree_built_3_for_test() -> tree<int> {
  /*
       ______1________
      /      |       \
     2       3        9
    /|\   _ /| \ __
   4 7 9 | 8 10 11 |
         | |       |
         | 9       | -> pruned
         |________ |

   The nodes kept (not pruned) are multiplied by 2

  */
  auto t4 = tree<int> {4*2};
  auto t7 = tree<int> {7*2};
  auto t9 = tree<int> {9*2};
  auto t2 = create_tree(2*2,t4,t7,t9);
  auto t3 = tree<int> {3*2};

  auto t1 = create_tree(1*2,t2,t3,t9);

  return t1;
}

TEST_CASE("create_pruned_tree_from_nodes") {
  io_adjacency_graph<int> g = create_io_graph_for_tests();
  auto root = rooted_view(g,8);

  auto t = create_pruned_tree_from_nodes(root,builder_3_for_test{});

  CHECK( t == tree_built_3_for_test() );
}
// create_pruned_tree_from_nodes }

// create_pruned_tree_from_adjacencies {
struct builder_4_for_test {
  using tree_type = tree<int>;
  using adjacency_type = io_adjacency<rooted_view<io_adjacency_graph<int>>>;
  auto
  should_go_down(const adjacency_type& adj) -> bool {
    return node(adj)!=3;
  }
  auto
  build(const adjacency_type& adj) -> tree<int> {
    auto sum = node(adj);
    if (should_go_down(adj)) {
      auto cs = children(adj);
      sum = std::accumulate(begin(cs),end(cs),sum,[](int i,const auto& t){ return i+node(t); });
    }
    return tree<int>(sum);
  }
};

auto
tree_built_4_for_test() -> tree<int> {
  /*
       ______1+2+3+9_
      /      |       \
     2+4+7+9 3        9
    /|\   _ /| \ __
   4 7 9 | 8 10 11 |
         | |       |
         | 9       | -> pruned
         |________ |
  */
  auto t4 = tree<int> {4};
  auto t7 = tree<int> {7};
  auto t9 = tree<int> {9};
  auto t2 = create_tree(2+4+7+9,t4,t7,t9);
  auto t3 = tree<int> {3};

  auto t1 = create_tree(1+2+3+9,t2,t3,t9);

  return t1;
}

TEST_CASE("create_pruned_tree_from_adjacencies") {
  io_adjacency_graph<int> g = create_io_graph_for_tests();
  auto root = rooted_view(g,8);

  auto t = create_pruned_tree_from_adjacencies(root,builder_4_for_test{});

  CHECK( t == tree_built_4_for_test() );
}
// create_pruned_tree_from_adjacencies }
