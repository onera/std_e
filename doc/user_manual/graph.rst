.. _graph:

******
Graphs
******

For notes and vocabulary regarding graphs in **std_e**, see :ref:`this section <graph_explained>`.

.. _adjacency_graph:

Adjacency graphs
================

simple_graph and io_simple_graph
--------------------------------

simple_graph
^^^^^^^^^^^^
An adjacency graph can be stored in a :cpp:`simple_graph`. The primary interface consists of three methods:

* :cpp:`nodes()`: the list of node values
* :cpp:`egdes()`: the adjacency list of edge values
* :cpp:`indices()`: the adjacency list of indices

Both are template classes and can be parameterized by:

* an node type :cpp:`NT`
* an edge type :cpp:`ET`
* an adjacency list type :cpp:`ALT`, defaulting to :cpp:`vector<vector<int>`.

For example, let us take the following graph:

.. graphviz::

  digraph G {
    graph [truecolor=true bgcolor="#00000000"]
    B -> A [label = "2."]
    B -> C [label = "3.14"]
    B -> D [label = "2.7"]
    C -> D [label = "10."]
    A -> A [label = "5."]
  }

It can be represented by a :cpp:`simple_graph<string,double>`. For that, we need to choose an order in which the nodes are listed. For instance :code:`[A,B,C,D]`. Then we would have:

* :cpp:`nodes() = [A,B,C,D]`
* :cpp:`egdes() = [ [5.], [2.,3.14,2.7], [10.], [] ]`
* :cpp:`indices() = [ [0], [0,2,3], [2], [] ]`

io_simple_graph
^^^^^^^^^^^^^^^
We can also use a :cpp:`io_simple_graph<string,double>` to reprensent the **same** graph. The primary interface of an :cpp:`io_simple_graph` is the following:

* :cpp:`nodes()`: the list of node values
* :cpp:`in_egdes()`: the adjacency list of inward edge values
* :cpp:`in_indices()`: the adjacency list of inward indices
* :cpp:`out_egdes()`: the adjacency list of outward edge values
* :cpp:`out_indices()`: the adjacency list of outward indices

In order to represent the graph, we can **choose** to store the edges in `out_edges` and `out_indices`:

* :cpp:`nodes() = [A,B,C,D]`
* :cpp:`out_egdes() = [ [5.], [2.,3.14,2.7], [10.], [] ]`
* :cpp:`out_indices() = [ [0], [0,2,3], [2], [] ]`
* :cpp:`in_egdes() = [ [], [], [], [] ]`
* :cpp:`in_indices() = [ [], [], [], [] ]`

This is **completely equivalent** from the :cpp:`simple_graph` representation. However, in many algorithms, we want to go back and forth between nodes sharing an edge. We our current choice of using only outward indices, this is not possible: I can be on node B and go to node C because I have the information in the adjacency list of B that I am connected to node C; but the reverse is not possible: once I am on node C, their is no mention of node B in the adjacency list of C. What we have is the equivalent of **forward iterators**, but for graphs. In order to go back and forth, we can store the other direction in `in_edges` and `in_indices`.

* :cpp:`nodes() = [A,B,C,D]`
* :cpp:`out_egdes() = [ [5.], [2.,3.14,2.7], [10.], [] ]`
* :cpp:`out_indices() = [ [0], [0,2,3], [2], [] ]`
* :cpp:`in_egdes() = [ [5.,2.], [], [3.14], [2.7,10.] ]`
* :cpp:`in_indices() = [ [0,1], [], [1], [1,2] ]`

Special case: no edge value
^^^^^^^^^^^^^^^^^^^^^^^^^^^
This is often the case that there is no edge value as in the following graph:

.. graphviz::

  digraph G {
    graph [truecolor=true bgcolor="#00000000"]
    B -> A
    B -> C
    B -> D
    C -> D
    A -> A
  }

In this case, the edge type can be chosen to be :cpp:`void`. No edge information is then stored. The :cpp:`edges()` method is then constant, and it returns :cpp:`indices()`.

Special case: no node value
^^^^^^^^^^^^^^^^^^^^^^^^^^^
Sometimes, no there is no node value. In this case, the node type can be chosen to be :cpp:`void`. No node information is then stored. The nodes are represented by their index as in the following graph. The :cpp:`nodes()` method is constant and returns :cpp:`std::iota_view(n)`, where :cpp:`n` is the number of nodes.

.. graphviz::

  digraph G {
    graph [truecolor=true bgcolor="#00000000"]
    1 -> 0 [label = "2."]
    1 -> 2 [label = "3.14"]
    1 -> 3 [label = "2.7"]
    2 -> 3 [label = "10."]
    0 -> 0 [label = "5."]
  }

Implementation note
^^^^^^^^^^^^^^^^^^^
In order to take all the special cases into account, :cpp:`simple_graph` and :cpp:`io_simple_graph` are implemented by inheriting their data and interface from mixin classes:

.. image:: /images/simple_graph_class_diag.svg


graph and io_graph
------------------

:cpp:`graph` and :cpp:`io_graph` are extensions of :cpp:`simple_graph` and :cpp:`io_simple_graph` with an additional interface to handle iteration more easily. We present here, in logical order, file by file, the different types that arise when iterating through graphs.


graph.hpp
^^^^^^^^^^^^^^^^^^^
:cpp:`graph` and :cpp:`io_graph` are ranges and can be iterated through node by node.

.. code:: c++

  [io_]_graph<NT,ET> : public [io_]simple_graph
    begin() -> adjacency_node_iterator
    end() -> adjacency_node_iterator
    operator[] -> [io_]adjacency

adjacency.hpp
^^^^^^^^^^^^^
Adjacency has reference semantics, it represents a reference to an adjacency of the graph, that is, a node of the graph plus its connectivities (nodes to which it is connected to).

.. code:: c++

  [io_]adjacency<GT> // GT: graph type, should be "[io_]graph"
    node()
    [in|out]_degree()
    [in|out]_edges()
    [in|out]_adjacencies() -> adjacency_range


adjacency_node_iterator.hpp
^^^^^^^^^^^^^^^^^^^^^^^^^^^
Iterator returned by :cpp:`[io_]graph.[begin|end]()`. Iterates through adjacencies node by node.

.. code:: c++

  adjacency_node_iterator<GT>
    operator++, operator+= ...
    operator* -> [io_]adjacency

adjacency_range.hpp
^^^^^^^^^^^^^^^^^^^
Range view returned by :cpp:`[io_]adjacency.[in|out]_adjacencies()`. Representes the adjacencies of an adjacency.

.. code:: c++

  adjacency_range<GT,[in|out]>
    begin() -> adjacency_edge_iterator
    end() -> adjacency_edge_iterator
    operator[] -> [io_]adjacency

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

Node 0 is connected to 1 and 2, so it forms the adjacency :code:`(0|1,2)`. 1 forms adjacency :code:`(1|3)` and 2 forms adjacency :code:`(2|3,4)`. So the adjacencies of adjacency :code:`(0|1,2)` is the adjacency range :code:`[ (1|3) , (2|3,4) ]`.

adjacency_edge_iterator.hpp
^^^^^^^^^^^^^^^^^^^^^^^^^^^
Iterator to iterate through an adjacency_range. Iterates through the adjacency of a node edge by edge.

.. code:: c++

  adjacency_node_iterator<GT>
    operator++, operator+= ...
    operator* -> [io_]adjacency

Other files
^^^^^^^^^^^

traits.hpp
~~~~~~~~~~
Various convenience type traits.

index_adjacency.hpp
~~~~~~~~~~~~~~~~~~~
Helper structs to build adjacency graphs (for unit tests in particular)

graph_algo.hpp
~~~~~~~~~~~~~~~~~~~~~~~~
Helper algorithms over adjacent graphs

rooted_graph.hpp
~~~~~~~~~~~~~~~~
Root the graph, so it can be used for rooted graph algorithms (DFS in particular)


Graph algorithms
================
TODO

.. _nested_tree:

Nested tree
===========
Nested tree can be used to represent tree structures:

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
