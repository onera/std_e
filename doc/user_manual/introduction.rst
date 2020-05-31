.. _introduction:

Introduction
============

Philosophy
----------

**std_e** tries to follow the original STL and offer efficient generic components. That is, a set of clearly defined functions, types and concepts that are as efficient as if written by hand, but flexible enought to handle various data structures and policies.

The principles followed by **std_e** are discussed in:

* `Alexander Stepanov: STL and its design principles <https://www.youtube.com/watch?v=COuHLky7E2Q>`_
* `Sean Parent: Three goals for better code <https://www.youtube.com/watch?v=qH6sSOr-yk8>`_
* Robert C. Martin: Clean Code - A Handbook of Agile Software Craftsmanship

Design choices and conventions of the library
---------------------------------------------

STL-like programming
^^^^^^^^^^^^^^^^^^^^

**std_e** programming style heavily relies on template and function parameters. Classes are used when a class invariant arises, but plain :cpp:`struct` are used otherwise. Free functions are prefered whenever possible. Inheritance is reduced to convenience usage not impacting the overall design. Run-time polymorphism (through :cpp:`virtual` functions or type erasure) is not used (not needed in this context). Exception are rarely used (rarely needed in this context).

Naming conventions
^^^^^^^^^^^^^^^^^^

Identifiers are separated with underscores (snake_case). Only concept identifiers begin with a capital letter. The rule is relaxed when there is no ambiguity, such as :cpp:`std::array<T,N>`.

.. note::

  A concept and a type modeling this concept may have the same name, except for the capital letter. Example: the :cpp:`Multi_array` concept is modeled by the :cpp:`multi_array` template class.

Macros are always written with capital letters, and most of the time they begin with :cpp:`STD_E_`.

.. _unit_tests:

Unit tests
----------

In **std_e**, unit tests serve a double purpose:

* automatically ensuring code functionnality and quality
* propose a clear, precise documentation of the different features. 
  
Unit tests are extensively integrated in this documentation. If a feature is available in file :code:`std_e/my_feature/my_file.hpp`, then the associated unit-tests is generally in file :code:`std_e/my_feature/test/my_file.test.cpp`

Unit tests are written using `doctest <https://github.com/onqtam/doctest>`_. **doctest** is a lightweight, `Catch <https://github.com/catchorg/Catch2>`_\ -like framework. Different tests can be grouped by :cpp:`SUBCASE` under a :cpp:`TEST_CASE`. All subcases of a test case are independent. Example:

.. literalinclude:: /../examples/doctest_subcase.test.cpp
  :language: C++

This will print:

.. code:: c++

  common part at the beginning
  part 0
  common part at the end
  common part at the beginning
  part 1
  common part at the end

Thas is, the common parts of a test case are executed once every subcase.

Subcases can be nested. 

Concepts
^^^^^^^^

Concepts are not available in the language in C++17. In **std_e**, they are mainly used as named requirements. A common example is the use of concept names as template parameter :

.. code:: c++

  template<class Array> auto 
  // the template parameter is supposed to model the Array concept
  my_array_function(Array& x) {
    // ...
  }

A SFINAE constraint is sometimes used...

.. code:: c++

  template<
    class Array,
    std::enable_if_t< is_array<Array> , int > =0
  > auto 
  my_array_function(Array& x) {
    // ...
  }

...but the syntax is so ugly that we are not so keen on using it every time. See also :ref:`type_traits_constraints`.

If the template parameter does not model any concept, its name is often suffixed by :cpp:`_type`:

.. code:: c++

  template<class some_parameter_type> auto 
  my_templatefunction(some_parameter_type& x) {
    // ...
  }


Miscellaneous
^^^^^^^^^^^^^

Signed integers
"""""""""""""""

Signed integers are prefered. The problem with unsigned integers arises when they are mixed with signed ones: conversions lead to surprising results.

But mixes happens as soon as math operations are used. The most common example is for array sizes. Array sizes must be positive, but they are most of the time used in combination with indices, and indices are signed. The standard library uses :cpp:`size_t` for sizes, but this is a design error (that has been acknowledged since, but is keeped for retro-compatibility). In **std_e**, sizes are signed integers.

Unsigned integer types are only used for bit manipulations (bitmasks, hashes...).

See also: `Jon Kalb - unsigned: A Guideline for Better Code <https://www.youtube.com/watch?v=wvtFGa6XJDU>`_.

to_string
"""""""""
Many objects can be converted to strings (in particular for display) with the :cpp:`std_e::to_string(const T& x)` function. 
