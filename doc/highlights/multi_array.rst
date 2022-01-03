.. _multi_array_highlights:

Introduction to multi-dimensional arrays
----------------------------------------

1.1. Include file
^^^^^^^^^^^^^^^^^

.. code:: c++

  #include "std_e/multi_array/multi_array.hpp"


1.2. Basics
^^^^^^^^^^^

Multi-dimensional arrays of dynamic size can be created with :cpp:`dyn_multi_array<T,rank>`:

.. literalinclude:: /../std_e/multi_array/multi_array/test/multi_array_types.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] dyn_multi_array {
  :end-before: [Sphinx Doc] dyn_multi_array }

If only a multi-dimensional array :ref:`view <view_type_def>` over some already allocated memory is needed, :cpp:`dyn_multi_array_view<T,rank>` can be used:

.. literalinclude:: /../std_e/multi_array/multi_array/test/multi_array_types.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] dyn_multi_array_view {
  :end-before: [Sphinx Doc] dyn_multi_array_view }

Multi-dimensional arrays can be constructed from dimensions and filled afterwards:

.. literalinclude:: /../std_e/multi_array/multi_array/test/multi_array_types.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] dyn_multi_array from dimensions {
  :end-before: [Sphinx Doc] dyn_multi_array from dimensions }

1.3. Sub-arrays
^^^^^^^^^^^^^^^

We often want to extract a sub-array of a multi-dimensional array. Depending on the shape of the original array and the shape wanted for the sub-array to be extracted, several interfaces are offered.

1.3.1. Sub-arrays of 2D arrays
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. literalinclude:: /../std_e/multi_array/multi_array/test/strided_array.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] 2D array sub-views {
  :end-before: [Sphinx Doc] 2D array sub-views }


1.3.2. Sub-arrays along one axis
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. literalinclude:: /../std_e/multi_array/multi_array/test/strided_array.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] strided_array with one index {
  :end-before: [Sphinx Doc] strided_array with one index }

1.3.3. Sub-arrays along multiple axes
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. literalinclude:: /../std_e/multi_array/multi_array/test/strided_array.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] strided_array with multiple indices {
  :end-before: [Sphinx Doc] strided_array with multiple indices }

1.4. Miscellaneous topics
^^^^^^^^^^^^^^^^^^^^^^^^^

The multi-dimensional array functionalities offered by **std_e** are described in more depth in :ref:`multi_dimensional_arrays`. We briefly mention here some important points.

1. Only :cpp:`dyn_multi_array<T,rank>` has been presented here. But other classes are available with more parametrization. In particular, :cpp:`fixed_multi_array<T,N0,N1...>` can be used for compile-time fixed size multi-dimensional arrays.
2. Arrays values are by default contiguous in memory and Fortran-ordered (also called "column major" or "co-lexicographic order"). Alternatives are available.
3. With Fortran order, contiguous (non-strided) sub-arrays are possible if the fixed axes are the rightmost ones.

