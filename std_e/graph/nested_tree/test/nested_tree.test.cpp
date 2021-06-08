#include "std_e/unit_test/doctest.hpp"

#include "std_e/graph/nested_tree/nested_tree.hpp"
#include "std_e/graph/nested_tree/nested_tree_algorithm_examples.hpp"
#include "std_e/graph/test_utils/nested_tree.hpp"


namespace {


using namespace std_e;

// [Sphinx Doc] nested_tree {
TEST_CASE("Nested tree principle") {
  auto t4 = nested_tree<int>{4};
  auto t7 = nested_tree<int>{7};
  auto t2 = create_tree(2,t4,t7);

  auto t3 = nested_tree<int>{3};

  auto t = create_tree(1,t2,t3);

  CHECK( t.size() == 5 );
  CHECK( root(t) == 1 );

  auto cs = children(t);
  CHECK( cs.size() == 2 );
  CHECK( cs[0] == t2 );
  CHECK( cs[1] == t3 );
}
// [Sphinx Doc] nested_tree }

TEST_CASE("Nested tree basic accessors") {
  auto t = create_nested_tree_for_tests();

  CHECK( t.size() == 9 );
  CHECK( t.root() == 1 );
  CHECK( root(t) == 1 );
}

TEST_CASE("Nested tree children") {
  auto t = create_nested_tree_for_tests();

  // children
  auto cs = t.children();
  CHECK( t.children() == children(t) );

  // number of children
  CHECK( cs.size() == 2 );
  CHECK( nb_children(t) == 2 );

  // each child
  auto c0 = cs[0];
  CHECK( c0.size() == 3 );
  CHECK( root(c0) == 2 );

  auto c1 = cs[1];
  CHECK( c1.size() == 5 );
  CHECK( root(c1) == 3 );

  CHECK( child(t,0) == c0 );
  CHECK( child(t,1) == c1 );
}

TEST_CASE("Nested tree append") {
  auto t = create_nested_tree_for_tests();

  auto c2 = nested_tree<int>{42};
  t.append_child(c2);

  // is appended as a child
  auto cs = children(t);
  CHECK( cs.size() == 3 );

  // nothing else changed
  auto c0 = cs[0];
  CHECK( c0.size() == 3 );
  CHECK( root(c0) == 2 );

  auto c1 = cs[1];
  CHECK( c1.size() == 5 );
  CHECK( root(c1) == 3 );

  // is appended correctly
  auto c2_inserted = cs[2];
  CHECK( c2_inserted.size() == 1 );
  CHECK( root(c2_inserted) == 42 );
}

TEST_CASE("Nested tree is_leaf") {
  auto t = create_nested_tree_for_tests();
  auto c0 = child(t,0);
  auto c1 = child(t,1);

  CHECK_FALSE( is_leaf(c0) );
  CHECK_FALSE( is_leaf(c1) );

  CHECK      ( is_leaf(child(c0,0)) );
  CHECK      ( is_leaf(child(c0,1)) );

  CHECK_FALSE( is_leaf(child(c1,0)) );
  CHECK      ( is_leaf(child(c1,1)) );
  CHECK      ( is_leaf(child(c1,2)) );
}

TEST_CASE("Nested tree to string") {
  auto t = create_nested_tree_for_tests();

  std::string expected_t_to_string =
    "1\n"
    "  2\n"
    "    4\n"
    "    7\n"
    "  3\n"
    "    8\n"
    "      9\n"
    "    10\n"
    "    11\n";
  CHECK( to_string(t)==expected_t_to_string );
}




class intrusive_dfs_visitor {
  public:
    auto
    pre(int n){
      nodes.push_back(n);
    }
    auto down() {}
    auto up(){}

    auto
    dfs_pre_nodes() -> const std::vector<int>& {
      return nodes;
    }
  private:
    std::vector<int> nodes = {};
};
TEST_CASE("nested_tree intrusive depth-first search with pre-order") {
  auto t = create_nested_tree_for_tests();

  intrusive_dfs_visitor f;
  nested_tree_dfs_with_preordering(t,f);

  std::vector<int> expected_dfs_pre_nodes = {1, 2,4,7, 3,8,9,10,11};
  CHECK( f.dfs_pre_nodes() == expected_dfs_pre_nodes );
}


} // anon
