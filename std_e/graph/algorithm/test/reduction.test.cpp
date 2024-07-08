#if __cplusplus > 201703L
#include "std_e/unit_test/doctest.hpp"

#include "std_e/graph/nested_tree/nested_tree.hpp"
#include "std_e/graph/algorithm/reduction.hpp"
#include "std_e/graph/test_utils/nested_tree.hpp"


namespace {


using namespace std_e;


struct node_with_level_for_test {
  int value;
  int reverse_level;
};

auto
to_string(const node_with_level_for_test& x) -> std::string {
  return std::to_string(x.value) + ", " + std::to_string(x.reverse_level);
}

auto
level(const node_with_level_for_test& x) {
  return x.reverse_level;
}

using nested_tree_with_level_for_tests = nested_tree<node_with_level_for_test>;

struct graph_with_levels_creator {
  public:
    using input_type = nested_tree<int>;
    using adj_type = typename input_type::node_adj_type;
    using reduced_type = nested_tree_with_level_for_tests;

    auto
    init(const input_type& t) -> reduced_type {
      int i = node(t);
      return reduced_type({i,0});
    }
    auto
    finish(reduced_type& parent, const reduced_type& child) -> void {
      parent.append_child(child);
    }
    auto
    accumulate(reduced_type& parent, const reduced_type& child) -> void {
      int& parent_level = node(parent).reverse_level;
      const int& child_level = node(child).reverse_level;
      parent_level = std::max(parent_level,child_level+1);
    }
};

TEST_CASE("Tree reverse_levels") {
  auto t = create_nested_tree_for_tests();
  /* Reminder:
         1               lvl 3
      /     \
     |        3          lvl 2
     |     /  |  \
     2     8  |   \      lvl 1
   /  \    |  |    \
  4    7   9  10   11    lvl 0
  */

  auto t_with_levels = depth_first_reduce(t,graph_with_levels_creator{});

  std::string expected_s =
    "1, 3\n"
    "  2, 1\n"
    "    4, 0\n"
    "    7, 0\n"
    "  3, 2\n"
    "    8, 1\n"
    "      9, 0\n"
    "    10, 0\n"
    "    11, 0\n";
  CHECK( to_string(t_with_levels) == expected_s );
}


} // anon
#endif // C++20
