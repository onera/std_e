#if __cplusplus > 201703L
#include "std_e/unit_test/doctest.hpp"

#include "std_e/graph/nested_tree/nested_tree.hpp"
#include "std_e/graph/test_utils/graph.hpp"
#include "std_e/graph/build/tree.hpp"

#include "std_e/graph/test_utils/nested_tree.hpp"
#include <numeric>
#include <iostream>

namespace {

using namespace std_e;

// create_tree_from_adjacencies - same tree structure {
struct builder_0_for_test {
  using tree_type = nested_tree<int>;
  //using from_adjacency_ref_type = typename tree_type::adjacency_type;
  auto
  build(const auto& adj) -> nested_tree<int> {
    return nested_tree<int>(node(adj));
  }
};

auto
tree_built_0_for_test() -> nested_tree<int> {
  /* Tree equivalent to the graph of create_rooted_graph_for_tests()
       ____1____
      /    |    \
     2     3     9
    /|\   /| \
   4 7 9 8 10 11
         |
         9
  */
  auto t4 = nested_tree<int>{4};
  auto t7 = nested_tree<int>{7};
  auto t9 = nested_tree<int> {9};
  auto t2 = create_tree(2,t4,t7,t9);

  auto t8 = create_tree(8,t9);

  auto t10 = nested_tree<int>{10};
  auto t11 = nested_tree<int>{11};
  auto t3 = create_tree(3,t8,t10,t11);

  auto t1 = create_tree(1,t2,t3,t9);

  return t1;
}

TEST_CASE("create_tree_from_adjacencies - same tree structure") {
  auto g = create_rooted_graph_for_tests();

  auto t = create_tree_from_adjacencies(g,builder_0_for_test{});

  CHECK( t == tree_built_0_for_test() );
}
// create_tree_from_adjacencies - same tree structure }


// create_tree_from_adjacencies - more complex tree structure {
struct builder_1_for_test {
  using tree_type = nested_tree<double>;
  auto
  build(const auto& adj ) -> nested_tree<double> {
    auto tl = nested_tree<double>(node(adj)*0.5);
    auto tr = nested_tree<double>(node(adj)*1.5);
    return create_tree(node(adj),tl,tr);
  }
};

auto
tree_init_1_for_test() -> nested_tree<int> {
  /*
        3
      /   \
      8   10
      |
      9
  */
  auto t9 = nested_tree<int> {9};
  auto t8 = create_tree(8,t9);

  auto t10 = nested_tree<int>{10};
  auto t3 = create_tree(3,t8,t10);

  return t3;
}
auto
tree_built_1_for_test() -> nested_tree<double> {
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
  auto t1_5 = nested_tree<double>{1.5};
  auto t4_5 = nested_tree<double>{4.5};

  auto t4 = nested_tree<double>{4.};
  auto t12 = nested_tree<double>{12.};

  auto t13_5 = nested_tree<double>{13.5};

  auto t5 = nested_tree<double>{5.};
  auto t15 = nested_tree<double>{15.};


  auto t9 = create_tree(9,t4_5,t13_5);
  auto t8 = create_tree(8,t4,t12,t9);

  auto t10 = create_tree(10,t5,t15);

  auto t3 = create_tree(3,t1_5,t4_5,t8,t10);

  return t3;
}


TEST_CASE("create_tree_from_adjacencies - more complex tree structure") {
  nested_tree<int> t_init = tree_init_1_for_test();

  auto t = create_tree_from_adjacencies(t_init,builder_1_for_test{});

  CHECK( t == tree_built_1_for_test() );
}
// create_tree_from_adjacencies - more complex tree structure }


// create_tree_from_adjacencies {
struct builder_2_for_test {
  using tree_type = nested_tree<int>;
  using adjacency_type = const_nested_tree_view<int>;
  auto
  build(const adjacency_type& adj) -> nested_tree<int> {
    auto sum = node(adj);
    auto cs = children(adj);
    sum = std::accumulate(begin(cs),end(cs),sum,[](int i,auto t){ return i+node(t); });
    return nested_tree<int>(sum);
  }
};
auto
tree_built_2_for_test() -> nested_tree<int> {
  /*
        3+8+10
      /   \
      8+9 10
      |
      9
  */
  auto t9 = nested_tree<int> {9};
  auto t8 = create_tree(8+9,t9);

  auto t10 = nested_tree<int>{10};
  auto t3 = create_tree(3+8+10,t8,t10);

  return t3;
}
TEST_CASE("create_tree_from_adjacencies") {
  nested_tree<int> t_init = tree_init_1_for_test();

  auto t = create_tree_from_adjacencies(t_init,builder_2_for_test{});

  CHECK( t == tree_built_2_for_test() );
}
// create_tree_from_adjacencies }


// create_pruned_tree_from_nodes {
struct builder_3_for_test {
  using tree_type = nested_tree<int>;
  using from_adjacency_ref_type = typename tree_type::reference;
  auto
  should_go_down(const auto& adj) -> bool {
    return node(adj)!=3;
  }
  auto
  build(const auto& adj) -> nested_tree<int> {
    return nested_tree<int>(node(adj)*2);
  }
};

auto
tree_built_3_for_test() -> nested_tree<int> {
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
  auto t4 = nested_tree<int> {4*2};
  auto t7 = nested_tree<int> {7*2};
  auto t9 = nested_tree<int> {9*2};
  auto t2 = create_tree(2*2,t4,t7,t9);
  auto t3 = nested_tree<int> {3*2};

  auto t1 = create_tree(1*2,t2,t3,t9);

  return t1;
}

TEST_CASE("create_pruned_tree_from_nodes") {
  auto g = create_rooted_graph_for_tests();

  auto t = create_pruned_tree_from_adjacencies(g,builder_3_for_test{});

  CHECK( t == tree_built_3_for_test() );
}
// create_pruned_tree_from_nodes }

// create_pruned_tree_from_adjacencies {
struct builder_4_for_test {
  using tree_type = nested_tree<int>;
  using adjacency_type = io_adjacency<rooted_graph<io_graph<int>>>;
  auto
  should_go_down(const adjacency_type& adj) -> bool {
    return node(adj)!=3;
  }
  auto
  build(const adjacency_type& adj) -> nested_tree<int> {
    auto sum = node(adj);
    if (should_go_down(adj)) {
      auto cs = children(adj);
      sum = std::accumulate(begin(cs),end(cs),sum,[](int i,const auto& t){ return i+node(t); });
    }
    return nested_tree<int>(sum);
  }
};

auto
tree_built_4_for_test() -> nested_tree<int> {
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
  auto t4 = nested_tree<int> {4};
  auto t7 = nested_tree<int> {7};
  auto t9 = nested_tree<int> {9};
  auto t2 = create_tree(2+4+7+9,t4,t7,t9);
  auto t3 = nested_tree<int> {3};

  auto t1 = create_tree(1+2+3+9,t2,t3,t9);

  return t1;
}

TEST_CASE("create_pruned_tree_from_adjacencies") {
  auto g = create_rooted_graph_for_tests();

  auto t = create_pruned_tree_from_adjacencies(g,builder_4_for_test{});

  CHECK( t == tree_built_4_for_test() );
}
// create_pruned_tree_from_adjacencies }

} // anonymous namespace
#endif // C++20
