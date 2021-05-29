#if __cplusplus > 201703L
#include "std_e/graph/adjacency_graph/merge_from_leaves.hpp"
#include "std_e/unit_test/doctest.hpp"

#include "std_e/graph/test_utils/io_graph.hpp"
#include "std_e/graph/adjacency_graph/adjacency_graph.hpp"


using namespace std_e;


struct S1 {
  char c;
  char h;
};
const auto eq_S = [](const S1& x, const S1& y){ return x.c == y.c; };
const auto less_S = [](const S1& x, const S1& y){ return x.c < y.c; };

auto
height(const S1& x) {
  return x.h;
}
auto
to_string(const S1& x) -> std::string {
  return {x.c};
}
auto
operator==(const S1& x, const S1& y) -> bool {
  return eq_S(x,y);
}
auto
operator<=>(const S1& x, const S1& y) {
  return x.c <=> y.c;
}
template<class I, class P> auto
my_sort(I f, I l, P p) {
  --l;
  if (p(*l,*f)) swap(*f,*l);
}

TEST_CASE("merge_from_leaves") {
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
       D (6)     A (7)     D (8)
       /\        /\        /\
      /  \      /  \      /  \
     A    B    A    C    A    B
    (0)  (1)  (2)  (3)  (4)  (5)
  */
  auto g = make_io_adjacency_graph<S1>({
   /* 0*/ {S1{'A',0}, {}, {}    },
   /* 1*/ {S1{'B',0}, {}, {}    },
   /* 2*/ {S1{'A',0}, {}, {}    },
   /* 3*/ {S1{'C',0}, {}, {}    },
   /* 4*/ {S1{'A',0}, {}, {}    },
   /* 5*/ {S1{'B',0}, {}, {}    },

   /* 6*/ {S1{'D',1}, {}, {0,1} },
   /* 7*/ {S1{'A',1}, {}, {2,3} },
   /* 8*/ {S1{'D',1}, {}, {4,5} },

   /* 9*/ {S1{'E',2}, {}, {6,7} },
   /*10*/ {S1{'F',3}, {}, {9,8} },
  });
  make_bidirectional_from_outward_edges(g);

  SUBCASE("sort_redirect_inward_to_equivalent") {
    std::vector<int> perm(g.size());
    std::iota(begin(perm),end(perm),0);

    auto eq_ = [eq=eq_S,&g](int i, int j){ return equivalent_by_node_and_outwards(g[i],g[j],eq); };
    auto less_ = [eq=eq_S,less=less_S,&g](int i, int j){ return less_by_node_and_outwards(g[i],g[j],eq,less); };

    sort_redirect_inward_to_equivalent(perm.data()  ,perm.data()+ 6,g,eq_,less_); // [0, 6) : level 0
    sort_redirect_inward_to_equivalent(perm.data()+6,perm.data()+ 9,g,eq_,less_); // [6, 9) : level 1
    sort_redirect_inward_to_equivalent(perm.data()+9,perm.data()+11,g,eq_,less_); // [0,11) : level 2

    std::vector<int> expected_perm = {
      //A,A,A,B,B,C
        0,2,4,1,5,3, // level 0
      //A,D,D
        7,6,8,       // level 1
      //E,F
        9,10         // level 2
    };
    CHECK( perm == expected_perm );

    std::vector<std::vector<int>> expected_out_indices = {
      //idx | node | old out_indices ->  new out_indices
      /* 0    A       {}             -> */ {}   ,
      /* 1    B       {}             -> */ {}   ,
      /* 2    A       {}             -> */ {}   ,
      /* 3    C       {}             -> */ {}   ,
      /* 4    A       {}             -> */ {}   ,
      /* 5    B       {}             -> */ {}   ,

      /* 6    D       {0,1}          -> */ {0,1},
      /* 7    A       {2,3}          -> */ {0,3},
      /* 8    D       {4,5}          -> */ {0,1},

      /* 9    E       {6,7}          -> */ {6,7},
      /*10    F       {9,8}          -> */ {9,6},
    };
    CHECK( g.out_indices() == expected_out_indices );
  }

  SUBCASE("merge_from_leaves") {
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
         D (4)     A (3)
         /\        /\
        /  \      /  \
    (0)A    B    /    C
       \   (1)  /    (2)
        \------/
    */
    auto expected_g = make_io_adjacency_graph<S1>({
     /*0*/ {S1{'A',0}, {}, {}    },
     /*1*/ {S1{'B',0}, {}, {}    },
     /*2*/ {S1{'C',0}, {}, {}    },

     /*3*/ {S1{'A',1}, {}, {0,2} },
     /*4*/ {S1{'D',1}, {}, {0,1} },

     /*5*/ {S1{'E',2}, {}, {4,3} },
     /*6*/ {S1{'F',3}, {}, {5,4} },
    });
    make_bidirectional_from_outward_edges(expected_g);

    CHECK( mg == expected_g );
  }
}
#endif // C++20
