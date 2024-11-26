.. _graph_explained:

Graphs
======

There are multiple ways to represent a graph.

Adjacency list
--------------
https://en.wikipedia.org/wiki/Adjacency_list

Description
"""""""""""
The graph is encoded by a range of adjacencies. An adjacency is a node + a range of nodes it is connected to.

Informally: **Adjacency_list == Range< Node,Range<Node> >**. In later sections, we refer to **Range< Node,Range<Node> >** as the **outer range**, and to the **Range<Node>** inside of it as the **inner range**.

Underlying ranges
"""""""""""""""""
The adjacency outer range can be anything (layout: array, list, deque...; ownership: container or view). Same for the inner range. The graph data structure will inherit from the properties of each (efficiency, complexity, iterator stability, ownership).

Since the adjacency list is actually a range of ranges, a compact way to store adjacencies is by using a :ref:`jagged array <jagged_array>`.

Orientation
"""""""""""
The adjacency can be **bidirectional** (if A is connected to B, then B is connected to A) or not. If all adjacencies are bidirectional, the graph is said to be a **bidirectional graph**. If no adjacency is bidirectional, the graph is said to be an **unidirectional graph**, which means a connection is stored only once.

The connection ("edge") can be oriented (then the edge is called an arc) or not. **Orientation is a property of the edge**; it is not necessarily linked to the notion of **bidirectionality, which is a data structure property**. The exact same graph can by encoded with:

* bidirectional connections and a flag indicating the direction of the connections,
* an unidirectional graph where by convention the connections are supposed to by the arcs going outward from the node,
* two ranges, one for inward connections, the other for outward connections.

From the base **data structure** point of view, there is only two true variations regarding orientation:

* either an inward range and outward range of adjacencies.
* or one range of adjacencies, where the user can either give a flag for orientation, or use a unidirectional graph with a convention.

Node and edge storage
"""""""""""""""""""""
The node storage is pretty straight-forward since the adjacency range is ordered by node. The only alternative is array-of-struct (AoS) vs struct-of-arrays (SoA): for each node, the node value can either be stored directly within the range, each node value along with the adjacencies of the node, or be stored in a separate range of the same size that can be zipped with the adjacency range.

The edge or arc can have a value (or not). For unidirectional graphs, the alternatives are the same ones as for the nodes. For other graphs however, the connection may be stored twice, and so the further alternative is either to store the edge value twice, or to store a pointer twice, with an auxiliary range (of size number of edges) storing the value. For arcs, most of the times the values are different so there is no problem.

Iteration
"""""""""
There are two ways of storing connection information: either by indices into the node range or by pointers/iterators. Storing iterators may seem more in line with usual STL data-structures, and is more flexible because it allows to handle list-like containers. However, it is complex to track off, because the adjacencies must then be rebuilt each time there is an iterator invalidation (in particular, node insertion if we use a :cpp:`std::vector`). This is why graph structures in **std_e** are storing indices, not pointers.


Edge list
---------
https://en.wikipedia.org/wiki/Edge_list

An edge list is a range of edges represented by the two nodes they are connecting. While their in also many possible alternative data structures to encode edge lists, we won't enter into the details. Most of the times, graphs are not stored in a edge list, the edge list is only created as a "supporting" data structure. As such, it is often just an array of node id pairs.

Adjacency matrix
----------------
https://en.wikipedia.org/wiki/Adjacency_matrix

Mesh-like structures
--------------------

A mesh is a special kind of graph where the nodes are called vertices. It can be encoded with the structures above. However, by doing so, an important information would be lost, that is, groups of nodes are gathered into "elements". The usual way to represent a mesh is by connectivity (note: we know no better technical term): a set of elements, where each element of the mesh is defined by its connectivity, i.e. an array of vertices (integers). This representation is often called "elt_vtx", "cell_vtx", "face_vtx" or "ridge_vtx" (a ridge is often called an edge, but we don't use this term for a ridge to not conflict with graph terminology). In general an elt_vtx is not a graph, but and hypergraph where vertices are the nodes and the elements are the multi-edges that gather all the vertices of the element. (Note: `elt_vtx` can also be considered multi-graph where elements are the nodes and the vertices are the edges [two elements can be connected through multiple vertices, hence it is not a graph], but it looses some information regarding the unicity of vertices [think of a 2x2 cartesian mesh represented this way: we can't tell easily that there is a center vertex if we only give edges connecting cells]).

Several (multi-)graph representations of a mesh are possible:
- elt_vtx, cell_vtx, face_vtx and ridge_vtx
- cell_face, elt_face, face_ridge and cell_ridge, elt_ridge similar to the previous ones, but instead of vertices, the elements are constructed from their lower dimension parts. It is possible to go from on representation to another. For example, a cell can be decomposed into its faces, so a "cell_face" and "face_vtx" can be deduced from a "cell_vtx".
- vtx_vtx, the adjacency list representation of the mesh. Not used much because no connectivity information.
- the edge list representation of the mesh. Not used. (note: edge list is equal to ridge_vtx if the ridges are segments)
- cell_cell or elt_elt, an adjacency list for the graph where the elements or the cells are the nodes of the graph.
- face_face, ridge_ridge same. Not used.
- face_cell or face_elt, the edge list version of cell_cell/elt_elt
- vtx_ridge, vtx_face, vtx_cell, vtx_elt, ridge_face, ridge_cell, ridge_elt

Note that several different cell_cell graphes can by created from one cell_vtx: by drawing edges beween cells sharing either a node, or an ridge, or a face. In cases where cells only touch by a corner for example (only share one single vertex), the cells are adjacent by vertices, but non-adjacent by faces or by ridges.

We can classify them into categories:

1. Adjacency list:
   vtx_vtx, cell_cell, elt_elt, (ridge_ridge, face_face)
2. Edge list:
   face_cell, face_elt (Represent the same graphs as respectively cell_cell and elt_elt. The edge list representations of vtx_vtx, ridge_ridge and face_face have no name)
3. (Lower) connectivity:
   ridge_vtx, face_vtx, cell_vtx, elt_vtx, face_ridge, cell_ridge, elt_ridge, cell_face, elt_face
4. Upper connectivity (same as connectivity, but the roles are switched):
   vtx_ridge, vtx_face, vtx_cell, vtx_elt, ridge_face, ridge_cell, ridge_elt [face_cell, face_elt are also here, but they are regular edge list structures]

The point to remember is that while categories 1 and 2 are regular graph representations, this is not the case for 3 and 4. They are actually multi-graphs, and while they are stored in structures similar to adjacency lists, these are not adjacency lists since the adjacenies are not nodes.


TODO replace previous with that:

   |cell_cell| , `face_cell` , 
   \cell_face\ , |face_face| ,
    cell_ridge ,  face_ridge , |ridge_ridge|, \vtx_ridge\
    cell_vtx   ,  face_vtx   , `ridge_vtx`  , |vtx_vtx|

1. Regular graphs represented by (node) adjacency lists (node_node):  vtx_vtx  ,  cell_cell,  ridge_ridge,  face_face
2. Regular graphs represented by edge lists             (edge_node):  ridge_vtx,  face_cell
3. Regular graphs represented by (edge) adjacency lists (node_edge):  vtx_ridge,  cell_face

4. Undirected hypergraphs represented by hyperedge lists (hedge_node):
   Remaining lower connectivities: face_vtx, cell_vtx, face_ridge, cell_ridge (here, face, cell are hyperedges)
   Remaining upper connectivities: vtx_face, vtx_cell, ridge_face, ridge_cell (here, vtx, ridge are hyperedges)

Notes:
- It is often implicitly supposed that `cell_cell` is the graph where cells are connected through faces (deduced from cell_face/face_cell), but we could also have a `cell_cell` connected through ridges or vtx (deduced respectively from cell_ridge/ridge_cell and cell_vtx/vtx_cell). 
- Only edge list representations (ridge_vtx, face_cell) don't need an index array (see :ref:`blocks`)
- Edge list representations are laid out in memory with all the "left" nodes, followed by the "right" nodes (no interweaving)
- Elements/High order
    - the extension of cell and faces to an element formalism is straightforward: one just need to carry a cell (or face) field that gives the element type (a number) of a cell (or face). One could specify ordering rules for not mixing different cell types together (e.g. TRI3 and QUAD8). Note that for higher orders, some (cell-interior) vertices may not be connected to any face/ridge.
    - the extension of ridges to higher order makes `ridge_vtx` need a `ridge_vtx_index`
    - it may be of interest to do many sub-computation on a degraded lower-order mesh. For instance, cell-renumbering does not go below the cell, so there is no interest in knowing that the cell actually has interior vertices.

  int          *cell_face;           /*!< Cell face connectivity
  int          *face_cell;           /*!< Face cell connectivity

  int          *face_edge;           /*!< Edge face connectivity
  int          *edge_face;           /*!< Edge face connectivity

  int          *face_vtx;            /*!< Face vertex connectivity
  int          *edge_vtx;           /*!< Face cell connectivity
