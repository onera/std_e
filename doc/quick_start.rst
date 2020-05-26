.. _quick_start:

.. currentmodule:: std_e

.. role:: cpp(code)
   :language: c++

Quick start
===========

.. contents:: :local:

Introduction
------------

**std_e** (for "standard extended") is a C++17 library. It contains general-purpose data structures and algorithms . Hopefully, one day, these functionalities will be provided by the C++ standard library.

The following documentation assumes a good knowledge of the C++ standard library and recent C++ constructs (most notably lambdas).

Installation
------------

**std_e** is header-only. See :ref:`installation` to build the tests, build the documentation or install the library.

Highlights
----------

Below is a succinct presentation of the main facilities of **std_e**. For a more detailed presentation, see the :ref:`user_manual`.

Everything in **std_e** is defined within the :cpp:`std_e` namespace, except macros. Generally, macros names are starting with :cpp:`STD_E_`.

In the examples below, when there is no abiguity, we will assume that we are using the :cpp:`std` and :cpp:`std_e` namespaces:

.. code:: c++

  using namespace std;
  using namespace std_e;

.. note::

  Most of the code snippets presented here are actually directly extracted from **std_e** unit tests. A :cpp:`TEST_CASE` allows to define a unit test. Each :cpp:`SUBCASE` is independent from previous ones at the same level: if a variable is changed in one section, the change is not kept in the following ones (for more details, see :ref:`unit_tests`). The :cpp:`CHECK` command allows to easily see what the expected result is supposed to be.


1. Multi-dimensional arrays
^^^^^^^^^^^^^^^^^^^^^^^^^^^

TODO

2. Heterogenous vector
^^^^^^^^^^^^^^^^^^^^^^

A :cpp:class:`std_e::hvector`\ :cpp:`<Ts...>` (short for heterogenous vector) is a wrapper around a :cpp:`std::tuple<std::vector<Ts...>>` with convenient access and algorithms.

2.1. Include file
"""""""""""""""""

.. code:: c++

  #include "std_e/data_structure/heterogenous_vector.hpp"

2.2. Basic usage
""""""""""""""""

.. literalinclude::  /../std_e/data_structure/test/heterogenous_vector.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] hvector {
  :end-before: [Sphinx Doc] hvector }

2.3. hvector algorithms
"""""""""""""""""""""""

2.3.1. for_each algorithms
~~~~~~~~~~~~~~~~~~~~~~~~~~

There is two levels possible levels for iterating an :cpp:`hvector`: :cpp:`for_each_vector` iterates through each vector of different type contained in the :cpp:`hvector`, while :cpp:`for_each_element` iterates through each individual element of each :cpp:`vector` in the :cpp:`hvector`.

Finally, :cpp:`for_each_element_if` iterates through an individual element only if it satisfies a predicate.

.. literalinclude::  /../std_e/data_structure/test/heterogenous_vector.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] hvector for_each algorithms {
  :end-before: [Sphinx Doc] hvector for_each algorithms }

2.3.2. find algorithms
~~~~~~~~~~~~~~~~~~~~~~

Ideally, algorithms on :cpp:class:`hvector` should be similar to the one used with any container, e.g. with :cpp:`std::vector`. However, when iterating over an :cpp:class:`hvector`, the underlying type of an element varies. That is why many standard algorithms are not directly applicable to an heterogenous vector.

This is the case with :cpp:`std::find_if`. Suppose we want to apply :cpp:`find_if` to :cpp:`hvector<int,string>`. The return value would be an iterator pointing to either an :cpp:`int` or a :cpp:`string`, which is not possible.

One way to get around this limitation is to apply the wanted operation as soon as we are iterating over the elements. Two algorithms are proposed to do that.
1. :cpp:`apply_first(hv,p,f)` iterates through an :cpp:class:`hvector` :cpp:`hv` in search of an elements satisfying predicate :cpp:`p`. Once it has found one such element, it applies function :cpp:`f` to it. It returns a boolean that tells if one such element has been found.
2. :cpp:`find_apply(hv,p,f)` does the same thing, but returns a pair :cpp:`[index_in_types,index_in_vector]` giving the postion of the element found.
3. :cpp:`find_position(hv,p)` is similar but it just returns the position and does not apply any function.

.. literalinclude::  /../std_e/data_structure/test/heterogenous_vector.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] hvector find algorithms {
  :end-before: [Sphinx Doc] hvector find algorithms }

3. STD_E_ENUM
^^^^^^^^^^^^^
C++ :cpp:`enum` and :cpp:`enum class` are missing two convenient features: the number of entries in the enum and conversions to :cpp:`std::string`. These features are provided by :cpp:`STD_E_ENUM` and :cpp:`STD_E_ENUM_CLASS` respectively. Example with :cpp:`STD_E_ENUM_CLASS`:

.. literalinclude::  /../std_e/utils/test/enum.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] STD_E_ENUM_CLASS {
  :end-before: [Sphinx Doc] STD_E_ENUM_CLASS }
