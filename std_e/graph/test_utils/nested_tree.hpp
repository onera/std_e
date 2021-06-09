#pragma once


#include "std_e/graph/nested_tree/nested_tree.hpp"


namespace std_e {


inline auto
create_nested_tree_for_tests() -> nested_tree<int> {
  /*
         1
      /     \
     2        3
   /  \    /  |  \
  4    7   8  10  11
           |
           9
  */
  auto t4 = nested_tree<int>{4};
  auto t7 = nested_tree<int>{7};
  auto t2 = create_tree(2,t4,t7);

  auto t9 = nested_tree<int> {9};
  auto t8 = create_tree(8,t9);

  auto t10 = nested_tree<int>{10};
  auto t11 = nested_tree<int>{11};
  auto t3 = create_tree(3,t8,t10,t11);

  auto t1 = create_tree(1,t2,t3);

  return t1;
}


} // std_e
