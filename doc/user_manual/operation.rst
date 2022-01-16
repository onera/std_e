.. _operation:

Math operations
===============

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
The :cpp:`operation_functor` types allow uniform operation calls:

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

