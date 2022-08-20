#if __cplusplus > 201703L
#include "std_e/unit_test/doctest.hpp"

#include "std_e/graph/nested_tree/nested_tree.hpp"
#include "std_e/graph/algorithm/algorithm.hpp"
#include "std_e/graph/test_utils/nested_tree.hpp"


using namespace std;
using namespace std_e;


TEST_CASE("Nested tree preorder depth-first find") {
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
  auto less_than_0  = [](const auto& x){ return root(x)<0 ; };
  auto less_than_2  = [](const auto& x){ return root(x)<2 ; };
  auto less_than_11 = [](const auto& x){ return root(x)<11; };
  auto less_than_12 = [](const auto& x){ return root(x)<12; };
  CHECK( !all_of_adjacencies(t,less_than_0 ) );
  CHECK( !all_of_adjacencies(t,less_than_2 ) );
  CHECK( !all_of_adjacencies(t,less_than_11) );
  CHECK(  all_of_adjacencies(t,less_than_12) );
}
#endif // C++20
