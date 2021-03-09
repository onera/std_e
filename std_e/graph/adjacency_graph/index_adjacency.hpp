#pragma once


#include <vector>


namespace graph {


template<class Int, template<class> class Range>
struct index_adjacency {
  Range<Int> connections;
};
template<class Int, template<class> class Range>
struct io_index_adjacency {
  Range<Int> inwards;
  Range<Int> outwards;
};




} // graph
