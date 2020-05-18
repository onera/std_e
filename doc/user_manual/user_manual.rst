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

  data_structure

*****************
Design philosophy
*****************

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

data_structure
================

graph
================

TODO

interval
================

iterator
================

memory_ressource
================

multi_index and multi_array
==============================

operation
================

parallel
================

polynomial
================

TODO

unit_test
================

Unit test utilities with doctest.
