.. _graph:

******
Graphs
******

For notes and vocabulary regarding graphs in **std_e**, see :ref:`this section <graph_explained>`.

.. _adjacency_graph:

Adjacency graph
===============

TODO


traits.hpp
----------

fwd decl and traits (+ TODO concepts)

adjacency_graph_base.hpp
------------------------

store the graph data
specialization for orientation/no orientation, edge/no edge, node/no node

.. graphviz::

  digraph G {
    node [shape=box fontname="consolas" fontsize="10"];

    al [ label=<
     <table border="0" cellborder="0" cellspacing="1">
       <tr><td align="center">      <b>adjacency_list_mixin</b>            </td></tr>
       <tr><td align="left">adjacency_list</td></tr>
     </table>>];

    ioal [label=<
     <table border="0" cellborder="0" cellspacing="1">
       <tr><td align="center"><b>io_adjacency_list_mixin</b>            </td></tr>
       <tr><td align="left">in_adjacency_list</td></tr>
       <tr><td align="left">out_adjacency_list</td></tr>
     </table>>];


    em [label=<
     <table border="0" cellborder="0" cellspacing="1">
       <tr><td align="center"><b>edge_mixin</b>            </td></tr>
       <tr><td align="left">edges</td></tr>
     </table>>];

    ioem [label=<
     <table border="0" cellborder="0" cellspacing="1">
       <tr><td align="center"><b>io_edge_mixin</b>            </td></tr>
       <tr><td align="left">in_edges</td></tr>
       <tr><td align="left">out_edges</td></tr>
     </table>>];

    nm [label=<
     <table border="0" cellborder="0" cellspacing="1">
       <tr><td align="center"><b>node_mixin</b>            </td></tr>
       <tr><td align="left">nodes</td></tr>
     </table>>];

    agb [label=<
     <table border="0" cellborder="0" cellspacing="1">
       <tr><td align="center"><b>adjacency_graph_base</b>            </td></tr>
     </table>>];

    ioagb [label=<
     <table border="0" cellborder="0" cellspacing="1">
       <tr><td align="center"><b>io_adjacency_graph_base</b>            </td></tr>
     </table>>];

    al -> em [arrowhead=onormal,arrowtail=onormal,dir=back]
    ioal -> ioem [arrowhead=onormal,arrowtail=onormal,dir=back]

    em -> agb [arrowhead=onormal,arrowtail=onormal,dir=back]
    nm -> agb [arrowhead=onormal,arrowtail=onormal,dir=back]
    ioem -> ioagb [arrowhead=onormal,arrowtail=onormal,dir=back]
    nm -> ioagb [arrowhead=onormal,arrowtail=onormal,dir=back]
  }




adjacency_graph.hpp
~~~~~~~~~~~~~~~~~~~

.. code::

  digraph G {

    [io_]adjacency_graph -> [io_]adjacency_graph_base
      begin -> adjacency_node_iterator
      end -> adjacency_node_iterator
      operator[] -> [io_]adjacency
  }


adjacency.hpp
~~~~~~~~~~~~~

Adjacency has reference semantics, it represents a reference to an adjacency of the graph, that is, a node of the graph plus its connectivities (nodes to which it is connected to).

.. code::

  digraph G {
    [io_]adjacency
      node
      [in|out]_degree
      [in|out]_edges
      [in|out]_adjacencies -> adjacency_range
  }

adjacency_node_iterator.hpp
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Iterator returned by :cpp:`[io_]adjacency_graph.begin|end`. Iterates through adjacencies node by node

adjacency_range.hpp
~~~~~~~~~~~~~~~~~~~

Range view returned by :cpp:`[io_]adjacency.[in|out]_adjacencies`. Representes the adjacencies of an adjacency.

Example:

.. graphviz::

  digraph G {
    graph [truecolor=true bgcolor="#00000000"]
    0 -> 1
    0 -> 2
    1 -> 3
    2 -> 3
    2 -> 4
  }

Node 0 is connected to 1 and 2, so it forms the adjacency :code:`(0|1,2)`. 1 forms adjacency :code:`(1|3)` and 2 forms adjacency :code:`(2|3,4)`. So the adjacencies of adjacency :code:`(0,1,2)` is the adjacency range :code:`[ (1|3) , (2|3,4) ]`.

adjacency_edge_iterator.hpp
~~~~~~~~~~~~~~~~~~~~~~~~~~~

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

.. _nested_tree:

Nested tree
===========

.. graphviz::

  digraph G {
    graph [truecolor=true bgcolor="#00000000"]
    1 -> 2
    1 -> 3
    2 -> 4
    2 -> 7
  }

.. literalinclude:: /../std_e/graph/nested_tree/test/nested_tree.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] nested_tree {
  :end-before: [Sphinx Doc] nested_tree }
