.. _quick_start:

Quick start
===========

.. currentmodule:: std_e

*******************************
Principals ideas of the library
*******************************


Concepts
========

**TODO**


Library design
==============

**TODO**

Tutorial
========


Simple example
--------------

Example of use of the :cpp:class:`std_e::hvector` (for "heterogenous vector ")

.. code:: c++

  std_e::hvector<int,double> hv = { vector{1,2,3,4} , vector{3.14,2.7} };
  int sz = 0;
  auto f = [&sz](auto v){ sz += v.size(); };
  for_each_vector(hv,f);
  // sz = 4+2 ;


Intro
-----------
**std_e** (for "std extended") is a C++17 library. It contains general-purpose data structures and algorithms (multi-dimensional array, constexpr std algorithms, utilities...). Hopefully, one day, these functionnalities will be provided by the C++ standard library.


Portability
-----------

The library is highly portable and relies only on a set of C++17 features:

* variadic templates
* type traits
* rvalue references
* decltype
* trailing return types
* deleted functions
* alias templates


Ease of Use
-----------

**std_e** has a small self-contained code base with the core library consisting of
just header files.

License
-------

Mozilla Public License Version 2.0

.. seealso:: This library is inspired of C++ standard library

##################
Indices and tables
##################

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
