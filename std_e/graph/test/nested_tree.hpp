#pragma once


#include "std_e/graph/tree/nested_tree.hpp"


namespace graph {


inline auto
create_nested_tree_for_tests() -> tree<int> {
  /*
         1
      /     \
     2        3
   /  \    /  |  \
  4    7   8  10  11
           |
           9
  */
  auto t4 = tree<int>{4};
  auto t7 = tree<int>{7};
  auto t2 = create_tree(2,t4,t7); 

  auto t9 = tree<int> {9};
  auto t8 = create_tree(8,t9); 

  auto t10 = tree<int>{10};
  auto t11 = tree<int>{11};
  auto t3 = create_tree(3,t8,t10,t11); 

  auto t1 = create_tree(1,t2,t3); 

  return t1;
}


} // graph
