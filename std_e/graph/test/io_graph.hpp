#pragma once


#include "std_e/graph/adjacency_graph/io_graph.hpp"
#include "std_e/graph/tree/nested_tree.hpp"


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


} // graph


namespace doctest {


template<class T> struct StringMaker<graph::io_graph<T>> {
  static String convert(const graph::io_graph<T>& x) {
    return to_string(x).c_str();
  }
};


} // graph
