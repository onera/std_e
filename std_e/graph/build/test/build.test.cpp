#include "std_e/unit_test/doctest.hpp"

#include "std_e/graph/build/build.hpp"
#include "std_e/graph/test_utils/graph.hpp"
#include "std_e/graph/adjacency_graph/graph.hpp"
#include "std_e/debug.hpp"
#include "std_e/graph/adjacency_graph/traits/traits.hpp"

using std_e::step;


//using namespace std_e;
//from maia.pytree.graph.build import depth_first_build_trees, depth_first_build
//from maia.pytree.graph.algo import step
//
//from maia.pytree.graph.f_graph import VALUE, rooted_f_graph_example, rooted_f_graph_with_indices_example, multiply_rooted_f_graph_example
//from maia.pytree.graph.fb_graph import BACKWARD


auto indent(const std::string& s) {
  std::string indented_s = "";
  auto lines = std_e::split(s,'\n');
  return "  " + std_e::join(lines, "\n  ") + "\n";
}

TEST_CASE("indent") {
  std::string s = "A\n  A0\n  A1\nB\nC\n";
  std::string expected_s = "  A\n    A0\n    A1\n  B\n  C\n";
  CHECK( indent(s) == expected_s );
}


TEST_CASE("depth_first_build_simple") {
  //   Reminder:
  //         1
  //      /  |  \
  //     |   |    3
  //     |   | /  |  \
  //     2\_ | 8  |   \
  //   /  \ \| |  |    \
  //   |  |  \ |  |    \
  //  4    7  \9  10   11

  auto g = std_e::create_rooted_graph_for_tests();

  auto indented_tree_ctor = [](const auto& adj, const auto& sub_strings) {
    auto res = std::to_string(node(adj)) + '\n';
    for (const auto& s : sub_strings) {
      res += indent(s);
    }
    return res;
  };

  SUBCASE("Test with a complete scan") {
    auto s = depth_first_build<std::string>(g, indented_tree_ctor);

    std::string expected_s =
      "1\n"
      "  2\n"
      "    4\n"
      "    7\n"
      "    9\n"
      "  3\n"
      "    8\n"
      "      9\n"
      "    10\n"
      "    11\n"
      "  9\n";

    CHECK( s == expected_s );
  }

  SUBCASE("Exit early") {
    auto step_over_2_and_out_10 = [](const auto& adj) {
      if (node(adj)==2) {
        return step::over;
      } else if (node(adj)==10) {
        return step::out;
      } else {
        return step::into;
      }
    };

    auto s = depth_first_build<std::string>(g, indented_tree_ctor, step_over_2_and_out_10);

    std::string expected_s =
      "1\n"
      "  2\n"
      "  3\n"
      "    8\n"
      "      9\n"
      "    10\n";

    CHECK( s == expected_s );
  }
}

class f_tree_ctor {
  public:
    // we initialize an empty `f_graph`
    f_tree_ctor()
    {}

    // each time we encounter an adjacency in our origin graph
    //   (which can happen multiple times for the same adjacency)
    //   we just create a new adjacency in our output graph
    auto operator()(const auto& adj, const auto& sub_node_indices) {
      f_graph.emplace_back(node(adj), sub_node_indices);
      int built_idx = f_graph.size()-1;
      return built_idx;
    }

    // return the graph we just built
    auto retrieve() {
      return std::move(f_graph);
    }
  private:
    std_e::graph<int> f_graph;
};

class fb_graph_ctor {
  public:
    // we will build a `fb_graph` instead of a `f_graph``
    //   just to prove that we can
    //   but this is irrelevant when comparing with the tree constructor
    fb_graph_ctor(const std_e::rooted_graph<std_e::index_io_graph<int>>& g)
      // for each adjacency in the origin graph
      //   we need to keep track of where it is stored in the output graph (if it has already been encountered)
      //     (we store `-1` if the adjacency has not been encountered yet)
      : visited(g.size(), -1)
    {}

    // Since we use a `tree_with_indices_adaptor`
    //   we also have the position of the adjacency in the origin graph
    // We use this position to index into `this->visited`
    //   so that if an adjacency is visited twice, we will index the same position in `this->visited`
    auto operator()(const auto& adj_with_idx, const auto& sub_node_indices) {
      //auto [adj, origin_idx] = adj_with_idx;
      auto& node_ref = node(adj_with_idx).ref;
      auto origin_idx = node(adj_with_idx).index;

      int built_idx;
      if (visited[origin_idx] == -1) {
        fb_graph.emplace_back(node_ref, std::vector<int>{}, sub_node_indices);
        built_idx = fb_graph.size()-1;
        visited[origin_idx] = built_idx;
      } else {
        built_idx = visited[origin_idx];
      }
      for (const auto& sub_node_idx : sub_node_indices) {
        fb_graph[sub_node_idx].in_indices().emplace_back( built_idx );
      }

      return built_idx;
    }

    // return the graph we just built
    auto retrieve() {
      return std::move(fb_graph);
    }
  private:
    std::vector<int> visited;
    std_e::index_io_graph<int> fb_graph;
};
//      return built_idx

TEST_CASE("depth_first_build") {
  //   Reminder:
  //         1
  //      /  |  \
  //     |   |    3
  //     |   | /  |  \
  //     2\_ | 8  |   \
  //   /  \ \| |  |    \
  //   |  |  \ |  |    \
  //  4    7  \9  10   11


  // We will build a new graph two ways
  // - First the easy way, by building a tree. Nodes with multiple parents will be duplicated in the resulting tree
  // - Second by preserving the DAG structure. There will be no nodes duplication

  SUBCASE("Building a tree") {
    // 0. create the original tree
    auto g = std_e::create_rooted_graph_for_tests();

    // 1. constructor
    f_tree_ctor f;

    // 2. call the algorithm
    depth_first_build<int>(g, f); // TODO get rid of explicit `int`
    auto res = f.retrieve();

    // 3. check
    //         1
    //      /  |  \
    //     |   |    3
    //     |   | /  |  \
    //     2\_ | 8  |   \
    //   /  \ \| |  |    \
    //   |  |  \ |  |    \
    //  4    7  \9  10   11
    std::string expected_res = // TODO to `index_adjacency_vector` rather than `to_string`
      /* 0*/ "{4, {}}\n"
      /* 1*/ "{7, {}}\n"
      /* 2*/ "{9, {}}\n"
      /* 3*/ "{2, {0,1,2,}}\n"
      /* 4*/ "{9, {}}\n"
      /* 5*/ "{8, {4,}}\n"
      /* 6*/ "{10, {}}\n"
      /* 7*/ "{11, {}}\n"
      /* 8*/ "{3, {5,6,7,}}\n"
      /* 9*/ "{9, {}}\n"
      /*10*/ "{1, {3,8,9,}}\n";
    CHECK( to_string(res) == expected_res );
  } // SUBCASE building a tree

  SUBCASE("Building a DAG") {
    // 0. create the original tree
    // Note: the adjacency list is exactly the same as previously,
    //       we don't need to allocate data differently
    //       only the interface changes to give move info
    auto g = std_e::create_rooted_index_graph_for_tests();

    // 1. constructor
    fb_graph_ctor f(g);

    // 2. call the algorithm
    depth_first_build<int>(g, f); // TODO get rid of explicit `int`
    auto res = f.retrieve();

    // 3. check
    //         1
    //      /  |  \
    //     |   |    3
    //     |   | /  |  \
    //     2\_ | 8  |   \
    //   /  \ \| |  |    \
    //   |  |  \ |  |    \
    //  4    7  \9  10   11
    std::string expected_res = // TODO to `io_index_adjacency_vector` rather than `to_string`
      /* 0*/ "{4, {3,}, {}}\n"
      /* 1*/ "{7, {3,}, {}}\n"
      /* 2*/ "{9, {3,4,8,}, {}}\n"
      /* 3*/ "{2, {8,}, {0,1,2,}}\n"
      /* 4*/ "{8, {7,}, {2,}}\n"
      /* 5*/ "{10, {7,}, {}}\n"
      /* 6*/ "{11, {7,}, {}}\n"
      /* 7*/ "{3, {8,}, {4,5,6,}}\n"
      /* 8*/ "{1, {}, {3,7,2,}}\n";
    CHECK( to_string(res) == expected_res );
  } // SUBCASE building a DAG
}



//auto test_depth_first_build_trees():
//  //   Reminder:
//  //    root  root
//  //     |     |
//  //     v     v
//  //
//  //     2\_   8
//  //   /  \ \  |
//  //   |  |  \ |
//  //  4    7  \9
//  g = multiply_rooted_f_graph_example()
//
//  auto indented_tree_ctor(adj, sub_strings):
//    return str(adj[VALUE]) + '\n' + ''.join([indent(s) for s in sub_strings])
//  s = depth_first_build_trees(g, indented_tree_ctor)
//
//  assert s == [('2\n'
//                '  4\n'
//                '  7\n'
//                '  9\n'
//               ),
//               ('8\n'
//                '  9\n'
//               )
//              ]
