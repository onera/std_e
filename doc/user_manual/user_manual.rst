.. _user_manual:

###########
User Manual
###########

.. currentmodule:: std_e


*****************
Table of contents
*****************

.. toctree::
  :maxdepth: 4


*****************
Design philosophy
*****************

TODO
Concepts
Integers: int and signed
Defaults: 0 and intervals

.. _unit_tests:

Unit tests
==========

TODO


******************
Folder description
******************

`base/`, `future/` and `utils/`
===============================

These are utility folders with mainly unrelated content. 

`base/` contains the most basic type aliases and functionnalities of the library.

`future` contains functionnality that is one the way of being integrated to C++, or is already integrated but not always supported by the toolchains:
* `constexpr` standard algorithms in `algorithm.hpp` and `constexpr_quick_sort.hpp`
* `constexpr_vector.hpp`: wrapper around `std::array` with `std::vector` semantics.


algorithm
================

TODO


data_structure
================

TODO


graph
================

TODO/UNFINISHED

interval
================

TODO


iterator
================

TODO


memory_ressource
================

TODO

.. _multi_array_user_manual:

multi_index and multi_array
===========================

Multi-dimensional arrays can be created with the :cpp:class:`std_e::multi_array` template class. :cpp:class:`std_e::multi_array` is not meant to be used directly, it can be specialized for different use cases. The most common uses are dynamic multi-dimensional arrays. :cpp:`std_e::dyn_multi_array<T,2>` is a multi-dimensional container which onw


operation
================

TODO


parallel
================

TODO


polynomial
================

TODO/UNFINISHED

unit_test
================

TODO

Unit test utilities with doctest.
