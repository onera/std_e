#pragma once


#include "std_e/graph/adjacency_graph/graph_algo.hpp"
#include "std_e/graph/nested_tree/nested_tree.hpp"
#include "std_e/graph/adjacency_graph/rooted_view.hpp"


namespace std_e {

inline auto
create_io_adjs_for_tests() -> io_index_adjacency_vector<int> {
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
  return io_index_adjacency_vector<int>({
    /*0*/ { 4, {2}    , {}     },
    /*1*/ { 7, {2}    , {}     },
    /*2*/ { 2, {8}    , {0,1,3}},
    /*3*/ { 9, {4,8,2}, {}     },
    /*4*/ { 8, {7}    , {3}    },
    /*5*/ {10, {7}    , {}     },
    /*6*/ {11, {7}    , {}     },
    /*7*/ { 3, {8}    , {4,5,6}},
    /*8*/ { 1, {}     , {2,7,3}},
  });
}

inline auto
create_io_graph_for_tests() -> io_graph<int> {
  return make_io_graph<int>(create_io_adjs_for_tests());
}
inline auto
create_rooted_graph_for_tests() -> rooted_graph<io_graph<int>> {
  return rooted_graph(create_io_graph_for_tests(),8);
}

inline auto
create_index_io_graph_for_tests() -> index_io_graph<int> {
  return make_index_io_graph<int>(create_io_adjs_for_tests());
}
inline auto
create_rooted_index_graph_for_tests() -> rooted_graph<index_io_graph<int>> {
  return rooted_graph(create_index_io_graph_for_tests(),8);
}

} // std_e


namespace doctest {


template<class T> struct StringMaker<std_e::io_graph<T>> {
  static String convert(const std_e::io_graph<T>& x) {
    return to_string(x).c_str();
  }
};


} // graph
