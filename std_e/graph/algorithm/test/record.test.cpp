#if __cplusplus > 201703L
#include "std_e/unit_test/doctest.hpp"

#include "std_e/graph/nested_tree/nested_tree.hpp"
#include "std_e/graph/algorithm/record.hpp"
#include "std_e/graph/test_utils/nested_tree.hpp"
#include "std_e/utils/functional.hpp"


using namespace std_e;


TEST_CASE("record_depth_first") {
  auto t = create_nested_tree_for_tests();
  /* Reminder:
         1
      /     \
     2        3
   /  \    /  |  \
  4    7   8  10  11
           |
           9
  */

  auto pre_nodes_0  = preorder_record_depth_first(t,std_e::identity);
  auto post_nodes_0 = postorder_record_depth_first(t,std_e::identity);
  auto [pre_nodes_1,post_nodes_1] = prepostorder_record_depth_first(t,std_e::identity,std_e::identity);

  std::vector<int> expected_pre_nodes = {1, 2,4,7, 3,8,9,10,11};
  std::vector<int> expected_post_nodes = {4,7,2, 9,8,10,11,3, 1};

  CHECK( pre_nodes_0  == expected_pre_nodes  );
  CHECK( pre_nodes_1  == expected_pre_nodes  );
  CHECK( post_nodes_0 == expected_post_nodes );
  CHECK( post_nodes_1 == expected_post_nodes );
}


int times_two(int i) { return 2*i; }
TEST_CASE("record_depth_first with non-identity function") {
  auto t = create_nested_tree_for_tests();
  auto pre_nodes = preorder_record_depth_first(t,times_two);

  std::vector<int> expected_pre_nodes = {2*1, 2*2,2*4,2*7, 2*3,2*8,2*9,2*10,2*11};

  CHECK( pre_nodes == expected_pre_nodes );
}
#endif // C++20
