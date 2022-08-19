#pragma once


#include <vector>


namespace std_e {

using connection_indices_container = std::vector<int>;

template<class T>
struct index_adjacency {
  T node;
  connection_indices_container indices;
};
template<class T> using index_adjacency_vector = std::vector<index_adjacency<T>>;

template<class T>
struct io_index_adjacency {
  T node;
  connection_indices_container inwards;
  connection_indices_container outwards;
};
template<class T> using io_index_adjacency_vector = std::vector<io_index_adjacency<T>>;

} // std_e
