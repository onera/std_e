#pragma once

#include <vector>


namespace std_e {
namespace test {


struct tree {
  int value;
  std::vector<tree> children;
};

inline auto first_child(      tree& t) -> auto { return t.children.begin(); }
inline auto first_child(const tree& t) -> auto { return t.children.begin(); }
inline auto  last_child(      tree& t) -> auto { return t.children.end  (); }
inline auto  last_child(const tree& t) -> auto { return t.children.end  (); }


auto
create_tree_for_tests() -> tree {
  /*
         1
      /     \
     2        3
   /  \    /  |  \
  4    7   8  10  11
           |
           9
  */
  return
    tree{1, {
      tree{2, {
        tree{4},
        tree{7}
      }},
      tree{3, {
        tree{8, {
          tree{9}
        }},
        tree{10},
        tree{11}
      }}}
    };
}

} // test
} // std_e
