#pragma once


#include <vector>


namespace graph {


template<class Int, template<class> class Range>
struct in_index_adjacency {
  Range<Int> inwards;
};
template<class Int, template<class> class Range>
struct out_index_adjacency {
  Range<Int> outwards;
};
template<class Int, template<class> class Range>
struct io_index_adjacency {
  Range<Int> inwards;
  Range<Int> outwards;
};


template<class T, template<class> class Range>
struct in_ptr_adjacency {
  Range<T*> inwards;
};
template<class T, template<class> class Range>
struct out_ptr_adjacency {
  Range<T*> outwards;
};
template<class T, template<class> class Range>
struct io_ptr_adjacency {
  Range<T*> inwards;
  Range<T*> outwards;
};



} // graph
