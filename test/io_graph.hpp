#pragma once


#include "graph/adjacency_graph/io_graph.hpp"
#include "graph/tree/nested_tree.hpp"


namespace graph {


using IC = connection_indices_container;


inline auto
create_io_graph_for_tests() -> io_graph<int> {
  /*
         1               lvl 3
      /  |  \
     |   |    3          lvl 2
     |   | /  |  \
     2\_ | 8  |   \      lvl 1
   /  \ \| |  |    \
   |  |  \ |  |    \
  4    7  \9  10   11    lvl 0
  */
  io_index_adjacency_vector<int> idx_adjs = {
    /*0*/ { 4,IC{2}    ,IC{}     },
    /*1*/ { 7,IC{2}    ,IC{}     },
    /*2*/ { 2,IC{8}    ,IC{0,1,3}},
    /*3*/ { 9,IC{4,8,2},IC{}     },
    /*4*/ { 8,IC{7}    ,IC{3}    },
    /*5*/ {10,IC{7}    ,IC{}     },
    /*6*/ {11,IC{7}    ,IC{}     },
    /*7*/ { 3,IC{8}    ,IC{4,5,6}},
    /*8*/ { 1,IC{}     ,IC{2,7,3}},
  };

  return io_graph<int>(std::move(idx_adjs));
}

inline auto
equivalent_tree_for_tests() -> tree<int> {
  /* Tree equivalent to the graph of create_io_graph_for_tests()
       ____1____
      /    |    \
     2     3     9
    /|\   /| \
   4 7 9 8 10 11
         |
         9
  */
  auto t4 = tree<int>{4};
  auto t7 = tree<int>{7};
  auto t9 = tree<int> {9};
  auto t2 = create_tree(2,t4,t7,t9); 

  auto t8 = create_tree(8,t9); 

  auto t10 = tree<int>{10};
  auto t11 = tree<int>{11};
  auto t3 = create_tree(3,t8,t10,t11); 

  auto t1 = create_tree(1,t2,t3,t9); 

  return t1;
}


} // graph


namespace doctest {


template<class T> struct StringMaker<graph::io_graph<T>> {
  static String convert(const graph::io_graph<T>& x) {
    return to_string(x).c_str();
  }
};


} // graph
