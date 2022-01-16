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


