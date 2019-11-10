#pragma once


#include "graph/tree/nested_tree.hpp"


namespace graph {


constexpr size_t nested_tree_max_size_for_tests = 128;
using nested_tree_for_tests = tree<int,nested_tree_max_size_for_tests>;


constexpr auto
create_nested_tree_for_tests() -> nested_tree_for_tests {
  /*
         1
      /     \
     2        3
   /  \    /  |  \
  4    7   8  10  11
           |
           9
  */
  auto t4 = nested_tree_for_tests{4};
  auto t7 = nested_tree_for_tests{7};
  auto t2 = create_tree(2,t4,t7); 

  auto t9 = nested_tree_for_tests {9};
  auto t8 = create_tree(8,t9); 

  auto t10 = nested_tree_for_tests{10};
  auto t11 = nested_tree_for_tests{11};
  auto t3 = create_tree(3,t8,t10,t11); 

  auto t1 = create_tree(1,t2,t3); 

  return t1;
}


} // graph
