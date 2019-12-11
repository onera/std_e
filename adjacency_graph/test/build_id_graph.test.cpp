#include "doctest/doctest.h"
#include "doctest/doctest_utils.hpp"
#include "graph/test/doctest_utils.hpp"

#include "graph/tree/nested_tree.hpp"
#include "graph/adjacency_graph/build_adjacency_graph.hpp"
#include "graph/test/utils.hpp"


using namespace graph;

struct int_height {
  int i;
  int height;
};
constexpr auto
operator==(const int_height& x, const int_height& y) -> bool {
  return x.i==y.i && x.height==y.height;
}
constexpr auto
operator!=(const int_height& x, const int_height& y) -> bool {
  return !(x==y);
}

struct int_height_builder {
  using result_type = int_height;

  template<class TTT> constexpr auto
  operator()(int x, int height, TTT outwards) -> int_height { // TODO
    return {x,height};
  }
};

TEST_CASE("build_bidirectional_graph") {
  auto t = create_nested_tree_for_tests();

  io_graph<int_height> io_g = build_bidirectional_graph(t,int_height_builder{});

  /* Reminder:
         1               lvl 3
      /     \
     |        3          lvl 2
     |     /  |  \
     2     8  |   \      lvl 1
   /  \    |  |    \
  4    7   9  10   11    lvl 0
  */
  using IC = connection_indices_container;
  io_index_adjacency_vector<int_height> expected_idx_adjs = {
    /*0*/ {{ 4,0},IC{2},IC{}     },
    /*1*/ {{ 7,0},IC{2},IC{}     },
    /*2*/ {{ 2,1},IC{8},IC{0,1}  },
    /*3*/ {{ 9,0},IC{4},IC{}     },
    /*4*/ {{ 8,1},IC{7},IC{3}    },
    /*5*/ {{10,0},IC{7},IC{}     },
    /*6*/ {{11,0},IC{7},IC{}     },
    /*7*/ {{ 3,2},IC{8},IC{4,5,6}},
    /*8*/ {{ 1,3},IC{} ,IC{2,7}  },
  };
  io_graph<int_height> expected_io_g(expected_idx_adjs);

  CHECK( io_g == expected_io_g );
}
