Adjacency graph
===============

traits.hpp
----------

fwd decl and traits (+ TODO concepts)

adjacency_graph_base.hpp
------------------------

store the graph data
specialization for orientation/no orientation, edge/no edge, node/no node

.. graphviz::

  digraph foo {

    adjacency_list_mixin
      adjacency_list

    io_adjacency_list_mixin
      in_adjacency_list
      out_adjacency_list

    edge_mixin -> adjacency_list_mixin
      edges
    io_edge_mixin -> io_adjacency_list_mixin
      in_edges
      out_edges

    node_mixin
      nodes

    adjacency_graph_base -> edge_mixin
    adjacency_graph_base -> node_mixin

    io_adjacency_graph_base -> io_edge_mixin
    io_adjacency_graph_base -> node_mixin
  }



adjacency_graph.hpp
~~~~~~~~~~~~~~~~~~~

.. graphviz::

  digraph foo {

    [io_]adjacency_graph -> [io_]adjacency_graph_base
      begin -> adjacency_node_iterator
      end -> adjacency_node_iterator
      operator[] -> [io_]adjacency
  }


adjacency.hpp
~~~~~~~~~~~~~

Adjacency has reference semantics, it represents a reference to an adjacency of the graph, that is, a node of the graph plus its connectivities (nodes to which it is connected to).

.. graphviz::

  digraph foo {

    [io_]adjacency
      node
      [in|out]_degree
      [in|out]_edges
      [in|out]_adjacencies -> adjacency_range
  }

adjacency_node_iterator.hpp
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Iterator returned by [io_]adjacency_graph.begin|end. Iterates through adjacencies node by node

adjacency_range.hpp
~~~~~~~~~~~~~~~~~~~

Range view returned by [io_]adjacency.[in|out]_adjacencies. Representes the adjacencies of an adjacency.

Example :

.. graphviz::

  digraph foo {
    0 -> 1
    0 -> 2
    1 -> 3
    2 -> 3
    2 -> 4
  }

Node 0 is connected to 1 and 2, so it forms the adjacency (0|1,2). 1 forms adjacency (1|3) and 2 forms adjacency (2|3,4). So the adjacencies of adjacency (0,1,2) is the adjacency range [ (1|3) , (2|3,4) ].

adjacency_connection_iterator.hpp
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Iterator to iterate through an adjacency_range. Iterates through the adjacency of a node edge by edge.


other files
-----------

index_adjacency.hpp
~~~~~~~~~~~~~~~~~~~

Helper structs to build adjacency graphs (for unit tests in particular)

adjacency_graph_algo.hpp
~~~~~~~~~~~~~~~~~~~~~~~~

Helper algorithms over adjacent graphs

rooted_graph.hpp
~~~~~~~~~~~~~~~~

Root the graph, so it can be used for rooted graph algorithms (DFS in particular)

