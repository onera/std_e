.. role:: cpp(code)
   :language: c++

.. _introduction:

Introduction
============

Design philosophy
-----------------

TODO
Concepts
Integers: int and signed
Defaults: 0 and intervals
Convention: snake_case
enable_XXX
to_string

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
