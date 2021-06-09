.. _interval:

Interval operations
===================

interval
--------
Intervals come in two flavors: closed-open or fully closed.

An :cpp:`interval<I>` models a closed-open interval. Closed-open interval `should be prefered <https://stackoverflow.com/a/9963438/1583122>`_ when possible.

.. literalinclude:: /../std_e/interval/test/interval.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] interval {
  :end-before: [Sphinx Doc] interval }

A :cpp:`closed_interval<I>` models a closed interval. Closed interval are useful primarily to interface with other software that is not using closed-open intervals.

.. literalinclude:: /../std_e/interval/test/interval.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] closed_interval {
  :end-before: [Sphinx Doc] closed_interval }

multi-interval
--------------

Multi-intervals are the extension of intervals in n dimensions. They represent rectangular sets. They are modeled by :cpp:`multi_interval<I>`. Multi-intervals are semi-open.

.. literalinclude:: /../std_e/interval/test/multi_interval.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] multi_interval {
  :end-before: [Sphinx Doc] multi_interval }

.. _interval_seq:

interval sequences
------------------

It is sometimes needed to represent an interval with points inside of it. For example, let's take interval :code:`[5,120)` and insert points :code:`10` and :code:`100`. All the information can then be represented by the interval sequence :code:`[5|10|100|120)`. Interval sequences can be modeled by the type :cpp:`interval_vector<I>`. It behaves very similarly to a :cpp:`std::vector`, except for the additional invariant that its values are stored in ascending order. The value type :cpp:`I` of an :cpp:`interval_vector` is supposed to be an arithmetic type (e.g. integer of floating point).

Overview of operations on interval vectors:

.. literalinclude:: /../std_e/interval/test/interval_sequence.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] interval_vector {
  :end-before: [Sphinx Doc] interval_vector }

Note that :cpp:`interval_vector` is a special case of :cpp:`interval_sequence<Random_access_range>`. Other possible types are e.g. the non-owning :cpp:`interval_span`.
