.. _mesh_data_struct :

Meshes
======

Note: we speak of a :def:`mesh` to refer to the discretization of a domain (which is what mesh is supposed to mean) AND the fields over the domain.

A mesh is composed of several fields:
- "regular" discretized physical fields (e.g. the pressure at the mesh vertices)
- miscellaneous informations over mesh entities (e.g. face ids)
- mesh connectivities can also be viewed as fields (e.g. `cell_face` is the field of faces that are connected to each cell)


In order to identify a mesh field, we need need to specify:
- its name
- its domain/connectivity type/entity kind/: is it a field over cell/face/ridge/vtx ?
- its support/topological support: does it span over all cells? or just a sub-region?


.. |                             | Type   | Entity | Support  | Layout                                             | Dimension                                          | Path/name |
.. |-----------------------------|--------|--------|----------|----------------------------------------------------|----------------------------------------------------|-----------|
.. |                             | int32  | cell   | complete | unit                                               | scalar (1)                                         |           |
.. |                             | int64  | face   | indexed  | block                                              | edge (2)                                           |           |
.. |                             | float  | ridge  |          | vblock                                             | ...                                                |           |
.. |                             | double | vtx    |          |                                                    | multiple (dyn)                                     |           |
.. | must be typed?              | yes    | no     | (yes)    | (yes)                                              | (yes)                                              |           |
.. | interest to treat together? | yes    | yes (renumber vtx in cell_vtx and face vtx)     | (no)     | (maybe, but need transparent view of block ranges) | (maybe, but need transparent view of multi ranges) |           |

// keys:
//    Field name
//    Domain kind: field on cell/face/ridge/vtx (4)
//
// types
//    Value type 
//    Domain span: complete/indexed (2) [+range (3)]
//    Layout: blocked/scalar/multiple (cell_face/ln_to_gn/face_cell or tensor) (3) [+constant block (4)]
//
// meta infos:
//    Field of cell/face/ridge/vtx/other (5)
//    Family name
//
//
