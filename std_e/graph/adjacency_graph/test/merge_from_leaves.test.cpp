#include "std_e/graph/adjacency_graph/merge_from_leaves.hpp"
#include "std_e/unit_test/doctest.hpp"

#include "std_e/graph/adjacency_graph/io_adjacency.hpp"
#include "std_e/graph/test/io_graph.hpp"
#include "std_e/log.hpp" // TODO


using namespace graph;

struct S {
  char c;
  char h;
};
const auto eq_S = [](const S& x, const S& y){ return x.c == y.c; };
const auto less_S = [](const S& x, const S& y){ return x.c < y.c; };

auto
height(const graph::io_adjacency<S>& x) {
  return x.node.h;
}
auto
to_string(const S& x) -> std::string {
  return {x.c};
}
auto
operator==(const S& x, const S& y) -> bool {
  return eq_S(x,y);
}

TEST_CASE("make_bidirectional_from_outward_edges") {
  /*
                 F (10)
                / \
               /   \
              /     \
             /       \
            E (9)     \
           / \         \
          /   \         \
         /     \         \
        /       \         \
       D (6)     D (7)     D (8)
       /\        /\        /\
      /  \      /  \      /  \
     A    B    A    C    A    B
    (0)  (1)  (2)  (3)  (4)  (5)
  */
  std_e::io_index_adjacency_vector<S> idx_adjs = {
   /* 0*/ {S{'A',0}, IC{} ,IC{}    },
   /* 1*/ {S{'B',0}, IC{} ,IC{}    },
   /* 2*/ {S{'A',0}, IC{} ,IC{}    },
   /* 3*/ {S{'C',0}, IC{} ,IC{}    },
   /* 4*/ {S{'A',0}, IC{} ,IC{}    },
   /* 5*/ {S{'B',0}, IC{} ,IC{}    },

   /* 6*/ {S{'D',1}, IC{} ,IC{0,1} },
   /* 7*/ {S{'D',1}, IC{} ,IC{2,3} },
   /* 8*/ {S{'D',1}, IC{} ,IC{4,5} },

   /* 9*/ {S{'E',2}, IC{} ,IC{6,7} },
   /*10*/ {S{'F',3}, IC{} ,IC{9,8} },
  };
  io_graph<S> g(idx_adjs);
  make_bidirectional_from_outward_edges(g);

  auto mg = merge_from_leaves(g,eq_S,less_S);

  /*
    Graph after factorization:
                 F (6)
                / \
               /   \
              /     \
             /      /
            E (5)  /
           / \    /
          /---\--/
         /     \
        /       \
       D (3)     D (4)
       /\        /\
      /  \      /  \
  (0)A    B    /    C
     \   (1)  /    (2)
      \------/
  */
  std_e::io_index_adjacency_vector<S> expected_idx_adjs = {
   /*0*/ {S{'A',0}, IC{} ,IC{}    },
   /*1*/ {S{'B',0}, IC{} ,IC{}    },
   /*2*/ {S{'C',0}, IC{} ,IC{}    },

   /*3*/ {S{'D',1}, IC{} ,IC{0,1} },
   /*4*/ {S{'D',1}, IC{} ,IC{0,2} },

   /*5*/ {S{'E',2}, IC{} ,IC{3,4} },
   /*6*/ {S{'F',3}, IC{} ,IC{5,3} },
  };
  io_graph<S> expected_g(expected_idx_adjs);
  make_bidirectional_from_outward_edges(expected_g);

  CHECK( mg == expected_g);
}
