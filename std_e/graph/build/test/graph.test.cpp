#include "std_e/unit_test/doctest.hpp"

#include "std_e/graph/nested_tree/nested_tree.hpp"
#include "std_e/graph/build/graph.hpp"
#include "std_e/graph/adjacency_graph/graph_algo.hpp"
#include "std_e/graph/test_utils/nested_tree.hpp"


using namespace std_e;


namespace {
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
  auto
  to_string(const int_height& x) -> std::string {
    return '('+std::to_string(x.i)+','+std::to_string(x.height)+')';
  }

  struct int_height_builder {
    using arg_type = int;
    using result_type = int_height;

    template<class TTT> constexpr auto
    operator()(int x, int height, TTT /*outwards*/) -> int_height { // TODO
      return {x,height};
    }
  };
}


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
