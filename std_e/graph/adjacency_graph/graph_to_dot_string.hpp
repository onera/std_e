#pragma once


#include "adjacency.hpp"
#include "graph.hpp"
#include <string>


namespace std_e {


// TODO CLEAN


template<class T, class F> auto
to_graphviz_node_string(int id, const io_adjacency<T>& x, F node_to_string_fun) -> std::string {
  using std::string;
  std::string label = node_to_string_fun(x.node());
  return std::to_string(id) + " [label=\""+label+"\"]";
}


//constexpr auto
//should_be_factored = [](const node_and_unique_id& x, const node_and_unique_id& y) {
//  if (node_kind(root(x.f).f)==generic_function_kind::constant
//   || node_kind(root(x.f).f)==generic_function_kind::param_extractor) return false;
//  if (node_kind(root(y.f).f)==generic_function_kind::constant
//   || node_kind(root(y.f).f)==generic_function_kind::param_extractor) return false;
//  return x.f==y.f;
//};


template<class N, class E, class C, class F> auto
to_string__impl(const io_graph<N,E,C>& g, F node_to_string_fun) -> std::string {
  std::string s = "digraph G {\n";
  for (int i=0; i<g.size(); ++i) {
    s += to_graphviz_node_string(i,g[i],node_to_string_fun) + '\n';
  }
  for (int i=0; i<g.size(); ++i) {
    const auto& node = g[i];
    const auto& deps = node.out_indices();
    for (auto dep : deps) {
      s += std::to_string(i);
      s += " -> ";
      s += std::to_string(dep);
      s += "\n";
    }
  }
  s += "}\n";
  return s;
}

//template<class T> auto
//to_string_bidir__impl(const io_graph<T>& g) -> std::string { // TODO factor with to_string__impl
//  std::string s = "digraph G {\n";
//  for (int i=0; i<g.adjs.size(); ++i) {
//    s += to_graphviz_node_string(i,g.adjs[i]) + '\n';
//  }
//  for (int i=0; i<g.adjs.size(); ++i) {
//    const auto& node = g.adjs[i];
//    const auto& deps = node.outward_indices;
//    for (int j=0; j<deps.size(); ++j) {
//      s += std::to_string(i);
//      s += " -> ";
//      s += std::to_string(deps[j]);
//      s += "\n";
//    }
//    //auto outs = node.out_node_ids;
//    //std::sort(begin(outs),end(outs));
//    //auto last = std::unique(begin(outs),end(outs));
//    //outs.resize(last-begin(outs));
//    const auto& outs = node.out_node_ids;
//    for (int j=0; j<outs.size(); ++j) {
//      s += std::to_string(i);
//      s += " -> ";
//      s += std::to_string(outs[j]);
//      s += "\n";
//    }
//  }
//  s += "}\n";
//  return s;
//}

//template<class T> auto
//repeat_constants_and_params(const io_graph<T>& g) {
//  io_graph<T> res_dg = g;
//  auto& ns = res_dg.adjs;
//
//  //// 0. remove constants and params
//  //int nb_removed = 0;
//  //for (int i=0; i<ns.size(); ++i) {
//  //
//  //}
//  int init_size = ns.size();
//  int new_size = ns.size();
//  for (int i=0; i<init_size; ++i) {
//    auto& deps = ns[i].outward_indices;
//    for (int j=0; j<deps.size(); ++j) {
//      int& dep = deps[j];
//      const auto& dep_node = ns[dep].node;
//      if (dep_node.is_constant()
//       || dep_node.is_param_extractor())
//      {
//        //ns.push_back(ns[dep]);
//        ns.push_back({-1,dep_node,{},{}}); // TODO HACK_0
//        dep = new_size;
//        ++new_size;
//      }
//    }
//  }
//  return res_dg;
//}


const auto to_string_lambda = [](const auto& x){ return to_string(x); };
const auto to_lite_string_lambda = [](const auto& x){ return to_lite_string(x); };

template<class N, class E, class C, class F = decltype(to_string_lambda)> auto
to_dot_format_string(const io_graph<N,E,C>& g, F node_to_string_fun = to_string_lambda) -> std::string {
  // io_graph<T> const_param_dg = repeat_constants_and_params(g); // HACK
  //return to_string__impl(const_param_dg);
  return to_string__impl(g,node_to_string_fun);
}

template<class T> auto
to_lite_dot_format_string(const io_graph<T>& g) -> std::string {
  return to_dot_format_string(g,to_lite_string_lambda);
}

//template<class T> auto
//to_dot_string_bidir(const io_graph<T>& g) -> std::string {
//  return to_string_bidir__impl(g);
//}


} // std_e
