
/**
  concept Node_adjacency
    Regular node_type
    Forward_iterator adjacency_iterator
  node(Node_adjacency) -> node_type 
  out_nodes(Node_adjacency) -> Range<Node_adjacency>

  concept Node_in_out_adjacency : Node_adjacency
  in_nodes(Node_adjacency) -> Range<Node_adjacency>


  concept Graph
    Node_adjacency node_adj_type

    first_child(Graph) -> Iterator<Node_adjacency>
    last_child(Graph) -> Iterator<Node_adjacency>

  concept Rooted_graph : graph
    first_root(Graph) -> iterator
    last_root(Graph) -> iterator


  concept Graph_adjacency_visitor
    Node_adjacency node_adj_type
    pre(node_adj_type)
    down(node_adj_type,node_adj_type)
    up(node_adj_type,node_adj_type)
    post(node_adj_type)


  concept Graph_node_visitor
    Regular node_type
    pre(node_type)
    down(node_type,node_type)
    up(node_type,node_type)
    post(node_type)
*/
