#if __cplusplus > 201703L
#include "std_e/unit_test/doctest.hpp"

#include "std_e/graph/build/convert_to_io_graph.hpp"
#include "std_e/graph/test_utils/nested_tree.hpp"
#include "std_e/graph/adjacency_graph/graph_algo.hpp"


using namespace std_e;


TEST_CASE("convert_to_io_graph") {
  auto t = create_nested_tree_for_tests();

  io_graph<int_height> io_g = convert_tree_to_io_graph_with_height(t);

  /* Reminder:
         1               lvl 3
      /     \
     |        3          lvl 2
     |     /  |  \
     2     8  |   \      lvl 1
   /  \    |  |    \
  4    7   9  10   11    lvl 0
  */
  auto expected_io_g = make_io_graph<int_height>({
    /*0*/ {{ 4,0}, {2}, {}     },
    /*1*/ {{ 7,0}, {2}, {}     },
    /*2*/ {{ 2,1}, {8}, {0,1}  },
    /*3*/ {{ 9,0}, {4}, {}     },
    /*4*/ {{ 8,1}, {7}, {3}    },
    /*5*/ {{10,0}, {7}, {}     },
    /*6*/ {{11,0}, {7}, {}     },
    /*7*/ {{ 3,2}, {8}, {4,5,6}},
    /*8*/ {{ 1,3}, {} , {2,7}  },
  });

  CHECK( io_g == expected_io_g );
}
#endif // C++20
