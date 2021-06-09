.. _multi_dimensional_arrays:

Multi-dimensional arrays
========================

.. _multi_array_intro:

Introduction
------------
There are many sources of variability regarding multi-dimensional arrays:

* Type of the values stored
* Memory allocation of the values
* Shapes of the array

  * rank
  * dimensions
  * offsets
  * dynamic or fixed sizes for all of these

* Memory layouts: continuous, segmented, strided, irregular
* Memory orders: Fortran/column-major, C/row-major, hybrid orders
* Views and subviews, slices, strides
* Conversions
* Supported operations (access, iteration, resizing...)


Vocabulary
----------
* The **axis** of a multi-dimensional array is the number of directions over which it spans.
* The **rank** of a multi-dimensional array is its number of axes. A `coefficient` is of rank `0`, an :cpp:`Array` (i.e. 1D-array) is of rank `1`, a 2D-array is of rank `2`.
* The **extent** of an array over an axis is the number of elements along this axis. For instance, the array :cpp:`{5,4,6}` has an extent of `3` over its only axis `0`.
* The **size** of an array is the product of its extents over all its axis.
* The **offset** over an axis it the number of elements before element `0` of that axis. By default, arrays have no offset (i.e. an offset of `0`).


Multi_index
-----------

Concept
^^^^^^^
A :cpp:`Multi_index` is an :cpp:`Array` whose :cpp:`value_type` is a :cpp:`std::signed_integral`. As its name implies, it is intended to be used as a multi-dimensional index. An **extent** is also a :cpp:`Multi_index`.

.. _multi_index:

multi_index<Int,N>
^^^^^^^^^^^^^^^^^^
The :cpp:`multi_index<Int,N>` class models the :cpp:`Multi_index` concept.

.. literalinclude:: /../std_e/multi_index/test/multi_index.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] multi_index {
  :end-before: [Sphinx Doc] multi_index }

The multi-index rank can be defined at run-time with :cpp:`multi_index<Int,std_e::dynamic_size>`.

.. literalinclude:: /../std_e/multi_index/test/multi_index.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] dynamic multi_index {
  :end-before: [Sphinx Doc] dynamic multi_index }

Multi_array_shape
-----------------

Concept
^^^^^^^
A :cpp:`Multi_array_shape` represents the shape of a multi-dimensional array, that is, what defines its **axes**. It has a **rank**, a **size**, a multi-index **extent** and a multi-index **offset**.

dyn_shape<Int,rank>
^^^^^^^^^^^^^^^^^^^
A :cpp:`dyn_shape<Int,rank>` is a :cpp:`Multi_array_shape` with a dynamic **extent**. If :cpp:`rank == std_e::dynamic_size`, its **rank** is also dynamic.

.. literalinclude:: /../std_e/multi_array/shape/test/dyn_shape.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] dyn_shape {
  :end-before: [Sphinx Doc] dyn_shape }

fixed_shape<Ns...>
^^^^^^^^^^^^^^^^^^
A :cpp:`fixed_shape<Ns...>` is a :cpp:`Multi_array_shape` with a fixed **extent**.

.. literalinclude:: /../std_e/multi_array/shape/test/fixed_shape.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] fixed_shape {
  :end-before: [Sphinx Doc] fixed_shape }

Note: A :cpp:`fixed_shape` always has an offset of zero.

Multi_array
-----------

Concept
^^^^^^^
A :cpp:`Multi_array` represents a multi-dimensional array. Like :cpp:`Multi_array_shape`, it has a **rank**, a **size**, an **extent** and an **offset**. But it also holds coefficients, accessible with :cpp:`operator()(Multi_index)` or :cpp:`operator()(Ints...)` where :cpp:`Ints...` are :cpp:`signed_integral` of multiplicity :cpp:`rank`.

multi_array<Contiguous_range,Multi_array_shape>
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
A :cpp:`multi_array` models the :cpp:`Multi_array` concept. It is parametrized by an underlying :cpp:`Contiguous_range` over memory and a :cpp:`Multi_array_shape`.

* The :cpp:`Contiguous_range` parameter allows different memory allocation strategies: a :cpp:`std::vector<T>`, a :cpp:`std::array<T,N>` or a :cpp:`span<T>` over external memory are possible choices. In the latter case, the :cpp:`multi_array` does not own its memory, and should be considered to be a **view**.
* The :cpp:`Multi_array_shape` parameter allows different shapes, mainly :cpp:`fixed_shape` and :cpp:`dyn_shape`.

The memory is supposed to be contiguous and Fortran-ordered (see below for alternatives)


fixed_multi_array, dyn_multi_array and dyn_multi_array_view
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
:cpp:`multi_array` is not very friendly to use. More convenient multi-dimensional arrays are defined from it:

* :cpp:`fixed_multi_array<T,Ns...>`

  * fixed rank and fixed dimensions
  * owns memory, allocated on the stack

.. literalinclude:: /../std_e/multi_array/multi_array/test/multi_array_types.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] fixed_multi_array {
  :end-before: [Sphinx Doc] fixed_multi_array }

* :cpp:`dyn_multi_array<T,rank,index_type>`

  * dynamic dimensions, fixed or dynamic rank (if :cpp:`rank==dynamic_size`)
  * owns memory (uses a :cpp:`std::vector<T>`)
  * :cpp:`index_type` must be chosen according to the size requirements (:cpp:`int32` or :cpp:`int64`). If not specified, it defaults to :cpp:`int` (:cpp:`int32` on most platforms)

.. literalinclude:: /../std_e/multi_array/multi_array/test/multi_array_types.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] dyn_multi_array {
  :end-before: [Sphinx Doc] dyn_multi_array }

* :cpp:`dyn_multi_array_view`

  * same as :cpp:`dyn_multi_array`...
  * ...but the :cpp:`Contigous_range` is a :cpp:`span<T>` (i.e. a pointer to external memory)
  * hence it is a **view**: it does not owns its elements, they are modified for the view if the owner modifies them, and vice-versa.

.. literalinclude:: /../std_e/multi_array/multi_array/test/multi_array_types.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] dyn_multi_array_view {
  :end-before: [Sphinx Doc] dyn_multi_array_view }


Views
^^^^^
:cpp:`dyn_multi_array_view` is just an example of a multi-dimensional array view. A view can be created from any :cpp:`multi_array` by calling `make_view`:

.. literalinclude:: /../std_e/multi_array/multi_array/test/multi_array_types.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] make_view {
  :end-before: [Sphinx Doc] make_view }

We will see later how we can create views for sub-arrays.

.. note::

  Creating a **view** is cheap because it only **refers** to coefficients and does not copy them. Hovewer, it is **not free**: creating a view requires to create/copy its **shape**. It can be detrimental in an inner loop. As an alternative, use a range of views (see :ref:`multi_array_iteration`)

For a discussion of view semantics, also see :ref:`val_ref_view`.

Constructors
^^^^^^^^^^^^

from extent
"""""""""""
.. literalinclude:: /../std_e/multi_array/multi_array/test/multi_array_ctors.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] multi_array constructors - from extent {
  :end-before: [Sphinx Doc] multi_array constructors - from extent }


from initialization list
""""""""""""""""""""""""
.. literalinclude:: /../std_e/multi_array/multi_array/test/multi_array_ctors.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] multi_array constructors - init lists {
  :end-before: [Sphinx Doc] multi_array constructors - init lists }


low-level constructors
""""""""""""""""""""""
.. literalinclude:: /../std_e/multi_array/multi_array/test/multi_array_ctors.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] multi_array constructors - low-level {
  :end-before: [Sphinx Doc] multi_array constructors - low-level }


Range interface
^^^^^^^^^^^^^^^
Multi-arrays also have a range interface, that essentially delegates every operation to its underlying range.

.. literalinclude:: /../std_e/multi_array/multi_array/test/multi_array_types.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] multi_array range interface {
  :end-before: [Sphinx Doc] multi_array range interface }


Sub-arrays
----------

Rank-reducing views
^^^^^^^^^^^^^^^^^^^
A **rank-reducing view** is created from a multi-dimensional array by **fixing one or several axes** at a specific (multi-)index. The most common examples would be to take a **row** or a **column** of a two-dimensional array.

Depending on the memory ordering of the multi-dimensional array, rank-reducing views can be **contiguous-memory preserving** or **strided**. For example, with Fortran order, columns are contiguous in memory, while rows are strided.

Contiguous-memory views
"""""""""""""""""""""""
With Fortran order, contiguous (non-strided) sub-arrays are possible if the fixed axes are the rightmost ones. In **std_e**, contiguous-memory sub-arrays are created with the :cpp:`make_sub_array` function.

.. literalinclude:: /../std_e/multi_array/multi_array/test/multi_array_types.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] make_sub_array {
  :end-before: [Sphinx Doc] make_sub_array }

1D sub-arrays views
~~~~~~~~~~~~~~~~~~~
If the number of fixed axes is equal to :cpp:`rank-1`, then the resulting view is 1D. That is, in case of a contiguous-memory view, we can create a :cpp:`span`. It can be done with the :cpp:`make_span` function.

.. literalinclude:: /../std_e/multi_array/multi_array/test/multi_array_types.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] make_span {
  :end-before: [Sphinx Doc] make_span }

.. note::

  Use :cpp:`make_span` when you are getting a 1D sub-array. More generally, always prefer "traditional" ranges (:cpp:`std::vector`, :cpp:`span`...) for 1D-arrays: their API is better-suited to range operations than a 1D multi-dimensional array is.

Strided-memory views
""""""""""""""""""""
Sometimes, it is not possible to create a contiguous sub-view. We must then fall back to creating **strided views**.

.. note::

  When you can, create contiguous sub-arrays with :cpp:`make_sub_array`. Adressing contiguous memory is always more efficient.


Strided view along one axis
~~~~~~~~~~~~~~~~~~~~~~~~~~~
.. literalinclude:: /../std_e/multi_array/multi_array/test/strided_array.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] strided_array with one index {
  :end-before: [Sphinx Doc] strided_array with one index }

Strided view along multiple axes
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
.. literalinclude:: /../std_e/multi_array/multi_array/test/strided_array.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] strided_array with multiple indices {
  :end-before: [Sphinx Doc] strided_array with multiple indices }

Columns and rows of 2D arrays
"""""""""""""""""""""""""""""
.. literalinclude:: /../std_e/multi_array/multi_array/test/strided_array.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] 2D array sub-views {
  :end-before: [Sphinx Doc] 2D array sub-views }

Rank-preserving views
^^^^^^^^^^^^^^^^^^^^^

Blocks
""""""
.. literalinclude:: /../std_e/multi_array/multi_array/test/block_view.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] block_view {
  :end-before: [Sphinx Doc] block_view }

Filters
"""""""
TODO
Filtered multi arrays are a refinement of blocks. Whereas blocks are constructed from sub-intervals of indices along each rank, filter views are defined by an arbitrary set of indices along each rank.

Rank-increasing views
^^^^^^^^^^^^^^^^^^^^^
TODO racks


Multi index range
-----------------

We often want to iterate through multi-indices in a specfic order (Fortran or C). A :cpp:`Multi_index_range` can be used just for that: it is a :cpp:`Range` that generates a :cpp:`Multi_index` at each iteration step.


Fortran order
^^^^^^^^^^^^^
.. literalinclude:: /../std_e/multi_index/test/multi_index_range.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] multi_index_range fortran order {
  :end-before: [Sphinx Doc] multi_index_range fortran order }


Arbitrary order
^^^^^^^^^^^^^^^
.. literalinclude:: /../std_e/multi_index/test/multi_index_range.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] multi_index_range arbitrary order {
  :end-before: [Sphinx Doc] multi_index_range arbitrary order }



.. _multi_array_iteration:

Iteration
---------

TODO

Algorithms
----------

TODO: fortran_order.hpp, increment_multi_index.hpp and strided_array/origin_indices


Miscellaneous
-------------

* There is no implicit conversion between :cpp:`multi_array` classes. To create a view, use :cpp:`make_view`.


Alternative memory order adapters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

TODO: multi_array_permuted_view.hpp

C-order is just Fortran-order with reversed indices, so use of C-order can be done by wrapping :cpp:`multi_array` into a class where accessors do reverse the indices. Same for other "hybrid" orders: just permute the indices.
