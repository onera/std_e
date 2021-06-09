.. _highlights:

.. currentmodule:: std_e

Highlights
==========

Below is a succinct presentation of the main facilities of **std_e**. For a more detailed presentation, see the :ref:`user_manual`.

In the examples below, when there is no ambiguity, we will assume that we are using the :cpp:`std` and :cpp:`std_e` namespaces:

.. code:: c++

  using namespace std;
  using namespace std_e;

.. note::

  Most of the code snippets presented here are actually directly extracted from **std_e** unit tests. A :cpp:`TEST_CASE` allows to define a unit test. Each :cpp:`SUBCASE` is independent from previous ones at the same level: if a variable is changed in one section, the change is not kept in the following ones (for more details, see :ref:`unit_tests`). The :cpp:`CHECK` command allows to easily see what the expected result is supposed to be.

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

.. _hvector:

Heterogenous vector
-------------------

A :cpp:`hvector<Ts...>` (short for heterogenous vector) is a wrapper around a :cpp:`std::tuple<std::vector<Ts...>>` with convenient access and algorithms.

2.1. Include file
^^^^^^^^^^^^^^^^^

.. code:: c++

  #include "std_e/data_structure/heterogenous_vector.hpp"

2.2. Basic usage
^^^^^^^^^^^^^^^^

.. literalinclude:: /../std_e/data_structure/test/heterogenous_vector.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] hvector {
  :end-before: [Sphinx Doc] hvector }

2.3. hvector algorithms
^^^^^^^^^^^^^^^^^^^^^^^

2.3.1. for_each algorithms
~~~~~~~~~~~~~~~~~~~~~~~~~~

There is two levels possible levels for iterating an :cpp:`hvector`:

1. :cpp:`for_each_range` iterates through each vector of different type contained in the :cpp:`hvector`
2. :cpp:`for_each_element` iterates through each individual element of each :cpp:`vector` in the :cpp:`hvector`.

Finally, :cpp:`for_each_element_if` iterates through an individual element only if it satisfies a predicate.

.. literalinclude:: /../std_e/data_structure/test/heterogenous_vector.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] hvector for_each algorithms {
  :end-before: [Sphinx Doc] hvector for_each algorithms }

2.3.2. find algorithms
~~~~~~~~~~~~~~~~~~~~~~

Ideally, algorithms on :cpp:class:`hvector` should be similar to the one used with any container, e.g. with :cpp:`std::vector`. However, when iterating over an :cpp:class:`hvector`, the underlying type of an element varies. That is why many standard algorithms are not directly applicable to an heterogenous vector.

This is the case with :cpp:`std::find_if`. Suppose we want to apply :cpp:`find_if` to :cpp:`hvector<int,string>`. The return value would be an iterator pointing to either an :cpp:`int` or a :cpp:`string`, which is not possible.

One way to get around this limitation is to apply the wanted operation as soon as we are iterating over the elements. Three algorithms are proposed to do that.

1. :cpp:`apply_first(hv,p,f)` iterates through an :cpp:`hvector` :cpp:`hv` in search of an elements satisfying predicate :cpp:`p`. Once it has found one such element, it applies function :cpp:`f` to it. It returns a boolean that tells if one such element has been found.
2. :cpp:`find_apply(hv,p,f)` does the same thing, but returns a pair :cpp:`[index_in_types,index_in_vector]` giving the postion of the element found.
3. :cpp:`find_position(hv,p)` is similar but it just returns the position and does not apply any function.

.. literalinclude:: /../std_e/data_structure/test/heterogenous_vector.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] hvector find algorithms {
  :end-before: [Sphinx Doc] hvector find algorithms }

2.4. Template argument deduction
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The template types of the :cpp:`hvector` can be deduced.

.. literalinclude:: /../std_e/data_structure/test/heterogenous_vector.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] hvector deduction {
  :end-before: [Sphinx Doc] hvector deduction }

.. note::

  It only works in C++20


2.5. Heterogenous range
^^^^^^^^^^^^^^^^^^^^^^^

A :cpp:`hvector<Ts...>` is actually just an alias to a :cpp:`hrange<vector,Ts...>`. That is, it is a particular case of a heterogenous range, and a :cpp:`hrange<Range,Ts...>` can be parametrized by other range types. For example, a :cpp:`hspan<Ts...>` is a :cpp:`hrange<span,Ts...>`.


.. _enum:

Enums convertible to strings
----------------------------

C++ :cpp:`enum` and :cpp:`enum class` are missing two convenient features: the number of entries in the enum and conversions to :cpp:`std::string`. These features are provided by :cpp:`STD_E_ENUM` and :cpp:`STD_E_ENUM_CLASS` respectively. Example with :cpp:`STD_E_ENUM_CLASS`:

.. literalinclude:: /../std_e/utils/test/enum.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] STD_E_ENUM_CLASS {
  :end-before: [Sphinx Doc] STD_E_ENUM_CLASS }
