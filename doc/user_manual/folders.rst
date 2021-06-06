.. _folders:

***********
Folder list
***********

We give here a brief description of **std_e** functionalities, folder by folder. To get more information, the reader is referred to the associated unit tests.

base/
=====

**base/** contains the most basic type aliases and functionalities of the library.

================================= =================================================================================
**macros.hpp**                    Definitions of :cpp:`FORCE_INLINE`, :cpp:`FORCE_NO_INLINE`, :cpp:`RESTRICT`
**inline_for.hpp**                Recursive implementation of a for loop that ensures it is inlined at compile time
**assertion_failure.hpp**         Exception class, to throw when a contract is not fulfilled
**not_implemented_exception.hpp** Exception class, to throw when a functionality is not implemented
**msg_exception.hpp**             Exception class, with no specific semantics
**lift.hpp**                      Convert a function template to a generic lambda. See :ref:`lift`
================================= =================================================================================

future/
=======
**future/** contains functionality that is one the way of being integrated to C++, or is already integrated but not always supported by the toolchains.

============================ =========================================================================================================
**span.hpp**                 Re-implementation :cpp:`std::span`
**algorithm.hpp**            :cpp:`constexpr` standard algorithms
**constexpr_quick_sort.hpp** :cpp:`constexpr std::sort`
**constexpr_vector.hpp**     Wrapper around :cpp:`std::array` with :cpp:`std::vector` semantics (:cpp:`push_back`, :cpp:`resize`...)
**contract.hpp**             Provides :cpp:`STD_E_ASSERT`, similar to :cpp:`assert`, but prints the stack trace
**zip.hpp**                  Provides the :cpp:`zip(Rng0& r0, Rng1& r1)` template function
============================ =========================================================================================================

utils/
======
=========================== ================================================================================================================
**array.hpp**               Common interface to create :cpp:`std::array` and :cpp:`std::vector` (needed in some generic contexts)
**concatenate.hpp**         Concatenate two ranges, append on range to another...
**enum.hpp**                See :ref:`enum`
**frozen_flat_map.hpp**     Key-value :cpp:`const` container implemented with two vectors
**functional.hpp**          :cpp`identity` and :cpp:`identity_closure`
**integer_range.hpp**
**pretty_print.hpp**        :cpp:`pretty_print_bytes`: human readable number of bytes (B, KB, MB...)
**string.hpp**              String utils: :cpp:`join`, :cpp:`split`, :cpp:`begins_with`, :cpp:`ends_with`
**switch.hpp**              See :ref`switch`. Given a run-time integer :cpp:`i`, :cpp:`switch_` dispatches to a function overload which first argument is :cpp:`std::integral_constant<int,i>`
**to_string.hpp**           :cpp:`to_string` function for common types, :cpp:`range_to_string`
**tuple.hpp**               Tuple algorithms: :cpp:`for_each`, :cpp:`find_apply`, :cpp:`for_each_until`
**vector.hpp**              Vector algorithms: :cpp:`sort`, :cpp:`unique`, :cpp:`sort_unique`, :cpp:`iota`, :cpp:`to_string`,
=========================== ================================================================================================================

.. _switch:

switch
------
TODO

parallel/
=========
MPI functions wrappers. TODO


operation/
==========
Represent mathematical operations by enumeration value, and bind them to their implementation. See :ref:`operation`.


algorithm/
==========
TODO


data_structure/
===============
TODO

multi_index/ and multi_array/
=============================
See :ref:`multi_array_highlights` and :ref:`multi_dimensional_arrays`


graph/
======

TODO/UNFINISHED

interval/
=========

Interval, interval sequence and multi-interval operations. See :ref:`interval`.

iterator/
=========
TODO


unit_test
=========
Unit test utilities with doctest.
