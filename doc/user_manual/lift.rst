.. _lift:

Lift
----

:cpp:`LIFT(f)` converts an overload set named **f** into a generic lambda, callable with exactly the same arguments that would be possible for **f**. Example:

.. literalinclude:: /../std_e/base/test/lift.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] lift {
  :end-before: [Sphinx Doc] lift }

