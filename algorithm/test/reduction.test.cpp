#include "doctest/doctest.h"
#include "doctest/doctest_utils.hpp"

#include "graph/tree/nested_tree.hpp"
#include "graph/algorithm/reduction.hpp"
#include "graph/algorithm/record.hpp"
#include "graph/test/nested_tree.hpp"


using namespace graph;


struct node_with_level_for_test {
  int value;
  int reverse_level;
};

auto
level(const node_with_level_for_test& x) {
  return x.reverse_level;
}

using nested_tree_with_level_for_tests = tree<node_with_level_for_test>;

struct graph_with_levels_creator {
  public:
    using node_type = int;
    using reducted_type = nested_tree_with_level_for_tests;

    auto
    init(int i) -> reducted_type {
      return reducted_type({i,0});
    }
    auto
    finish(reducted_type& parent, const reducted_type& child) -> void {
      parent.append_child(child);
    }
    auto
    accumulate(reducted_type& parent, const reducted_type& child) -> void {
      int& parent_level = node(parent).reverse_level;
      const int& child_level = node(child).reverse_level;
      parent_level = std::max(parent_level,child_level+1);
    }
};

TEST_CASE("Tree reverse_levels") {
  auto t = create_nested_tree_for_tests();

  auto t_with_levels = depth_first_reduce(t,graph_with_levels_creator{});

  /* Reminder:
         1               lvl 3
      /     \
     |        3          lvl 2
     |     /  |  \
     2     8  |   \      lvl 1
   /  \    |  |    \
  4    7   9  10   11    lvl 0
  */
  std::vector<int> reverse_levels = preorder_record_depth_first(t_with_levels,level);
  std::vector<int> expected_reverse_levels = {3, 1,0,0, 2,1,0,0,0};

  CHECK( reverse_levels == expected_reverse_levels);
}
