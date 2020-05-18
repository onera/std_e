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


multi_index and multi_array
==============================

TODO


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
