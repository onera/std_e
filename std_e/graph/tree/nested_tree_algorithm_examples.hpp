#pragma once


#include "std_e/graph/tree/nested_tree.hpp"


namespace graph {


template<class T, class M> auto
to_string__recursive_dfs_with_preordering(const nested_tree<T,M>& t, std::string indent = "") -> std::string {
  using std::to_string;
  std::string s = indent+to_string(root(t))+"\n";
  for (const auto& c : children(t)) {
    s += to_string__recursive_dfs_with_preordering(c,indent+unit_indent);
  }
  return s;
}
template<class T, class M> auto
to_string__recursive_dfs_with_postordering(const nested_tree<T,M>& t, std::string indent = "") -> std::string {
  using std::to_string;
  std::string s = "";
  for (const auto& c : children(t)) {
    s += to_string__recursive_dfs_with_postordering(c,indent+unit_indent);
  }
  return s+indent+to_string(root(t))+"\n";;
}


} // graph
