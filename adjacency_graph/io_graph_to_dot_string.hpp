#pragma once


namespace graph {


// TODO CLEAN


template<class T> auto
to_graphviz_node_string(int id, const io_adjacency<T>& x) -> std::string {
  using std::string;
  std::string label = to_string(x.node);
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


template<class T> auto
to_string__impl(const io_graph<T>& g) -> std::string {
  std::string s = "digraph G {\n";
  for (size_t i=0; i<g.size(); ++i) {
    s += to_graphviz_node_string(i,g[i]) + '\n';
  }
  for (size_t i=0; i<g.size(); ++i) {
    const auto& node = g[i];
    const auto& deps = node.outwards;
    for (size_t j=0; j<deps.size(); ++j) {
      s += std::to_string(i);
      s += " -> ";
      s += std::to_string(deps[j]-g.begin());
      //s += std::to_string(deps[j]->index);
      s += "\n";
    }
  }
  s += "}\n";
  return s;
}

//template<class T> auto
//to_string_bidir__impl(const io_graph<T>& g) -> std::string { // TODO factor with to_string__impl
//  std::string s = "digraph G {\n";
//  for (size_t i=0; i<g.adjs.size(); ++i) {
//    s += to_graphviz_node_string(i,g.adjs[i]) + '\n';
//  }
//  for (size_t i=0; i<g.adjs.size(); ++i) {
//    const auto& node = g.adjs[i];
//    const auto& deps = node.outward_indices;
//    for (size_t j=0; j<deps.size(); ++j) {
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
//    for (size_t j=0; j<outs.size(); ++j) {
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
//  //for (size_t i=0; i<ns.size(); ++i) {
//  //  
//  //}
//  size_t init_size = ns.size();
//  size_t new_size = ns.size();
//  for (size_t i=0; i<init_size; ++i) {
//    auto& deps = ns[i].outward_indices;
//    for (size_t j=0; j<deps.size(); ++j) {
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

template<class T> auto
to_dot_format_string(const io_graph<T>& g) -> std::string {
  // io_graph<T> const_param_dg = repeat_constants_and_params(g); // HACK
  //return to_string__impl(const_param_dg);
  return to_string__impl(g);
}

//template<class T> auto
//to_dot_string_bidir(const io_graph<T>& g) -> std::string {
//  return to_string_bidir__impl(g);
//}


} // graph
