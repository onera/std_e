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
**lift.hpp**                      See :ref:`lift`
================================= =================================================================================

.. _lift:

lift
----

:cpp:`LIFT(f)` converts an overload set named **f** into a generic lambda, callable with exactly the same arguments that would be possible for **f**. Example:

.. literalinclude:: /../std_e/base/test/lift.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] lift {
  :end-before: [Sphinx Doc] lift }

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
The :cpp:`operation_kind` enumeration lists many mathematical operations (+,-, sqrt, cos, tensor_prod, grad...). It allows to represent an operation by a simple value. This property can be used in contexts where we want to apply algorithms to operations in a uniform fashion, such as expression graphs.

operation_functor
-----------------
At some point, we may want to **call** the operation encoded by an :cpp:`operation_kind`. This is possible with :cpp:`operation_functor<operation_kind>`:

.. literalinclude:: /../std_e/operation/test/operation_functor.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] operation_functor principle {
  :end-before: [Sphinx Doc] operation_functor principle }

For each :cpp:`operation_kind` named :cpp:`my_op`, :cpp:`operation_functor<operation_kind>(args...)` will call the function :cpp:`my_op(args...)`.

The binding is the following: the enumeration value named :cpp:`my_op` in :cpp:`operation_kind` **is the same name** as the function called by :cpp:`operation_functor<operation_kind>`. The only exception is for operators: for example :cpp:`operation_functor<operation_kind::plus>(args...)` will call :cpp:`operator+(args...)`, not :cpp:`plus(args...)`.


Calling operations
------------------
The :cpp:`operation_functor` types allow uniform that operation calls:

.. literalinclude:: /../std_e/operation/test/operation_functor.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] operation_functor gen {
  :end-before: [Sphinx Doc] operation_functor gen }

However, :cpp:`operation_functors` depend on a compile-time :cpp:`operation_kind`. So it is not directly possible, for instance, to loop at run-time through a sequence of :cpp:`operation_kind` to call the associated function. However, we can use :cpp:`apply_operation` in order to do that:

.. literalinclude:: /../std_e/operation/test/dyn_op_functor.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] apply_operation {
  :end-before: [Sphinx Doc] apply_operation }

The problem of promoting a run-time :cpp:`operation_kind` to a compile-time value in order to call a matching function is a very similar problem to what is done with :ref:`switch`.

algorithm/
==========

TODO


data_structure/
===============

multi_index/
============
See :ref:`multi_array_highlights` and :ref:`multi_dimensional_arrays`

multi_array/
============
See :ref:`multi_array_highlights` and :ref:`multi_dimensional_arrays`


graph/
======

TODO/UNFINISHED

interval/
=========

TODO


iterator/
=========
TODO


unit_test
=========
Unit test utilities with doctest
