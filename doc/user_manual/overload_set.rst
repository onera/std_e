.. _overload_set:

Overload set
------------

:cpp:`STD_E_OVERLOAD_SET(f)` converts an overload set named **f** into a generic lambda, callable with exactly the same arguments that would be possible for **f**. Example:

.. literalinclude:: /../std_e/base/test/overload_set.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] overload_set {
  :end-before: [Sphinx Doc] overload_set }

