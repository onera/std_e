.. _pivot_partitions:

Pivot partitions
----------------


(1)  .. code-block:: c++

       template<class RA_it, class RA_it_piv, class RA_it_out>
       void pivot_partitions(RA_it first, RA_it last,
                             RA_it_piv piv_first, RA_it_piv piv_last,
                             RA_it_out out_first);

(2)  .. code-block:: c++

       template<class RA_it, class RA_it_piv, class RA_it_out,
                class Bin_pred>
       void pivot_partitions(RA_it first, RA_it last,
                             RA_it_piv piv_first, RA_it_piv piv_last,
                             RA_it_out out_first,
                             Bin_pred comp);

(1) Partitions the range [first,last) according to the pivots [piv_first,piv_last).

    This means that all the elements of [first,last) smaller than :cpp:`*piv_first` will come first, then all the elements smaller than :cpp:`*(piv_first+1)` will come second... etc.

    Let us write :cpp:`n = std::distance(first,last)` and :cpp:`k = std::distance(piv_first,piv_last)`.

    Range [first,last) is partitioned into k+1 buckets of ever greater elements.

    Each partition point, that is, each iterator separating the buckets, is written to the output range [out_first,k).

    The partition is not stable: the order of equal elements is not guaranteed to be preserved.

(2) Same as (1) but use of function comp to compare elements of the range to the pivots.

    comp is a binary predicate. Its first argument is supposed to be an element in the range [first,last), and its second argument is supposed to be an element in the range of pivots [piv_first,piv_last).

Parameters
^^^^^^^^^^

first, last
  the range of elements to partition

piv_first, piv_last
  the range of pivots that will be used to partition [first,last)

out_first
  the beginning of the destination range

  the range [out_first,k) is supposed to be valid

comp
  comparison function object (i.e. an object that satisfies the requirements of Compare) which returns  true if the first argument is less than (i.e. is ordered before) the second.


Type requirements
^^^^^^^^^^^^^^^^^

RA_it, RA_it_piv
  must meet the requirements of a RandomAccessIterator.


Bin_pred
  The signature of the comparison function should be equivalent to the following:

.. code-block:: c++

  bool comp(const T0& x, const T1& y);

where :cpp:`using T0 = RA_it::value_type` and :cpp:`using T1 = RA_it_piv::value_type`

RA_it_out
  must meet the requirements of a RandomAccessIterator.

  :cpp:`RA_it_out::value_type` should be the same as :cpp:`RA_it`

Return value
^^^^^^^^^^^^

(none)

Complexity
^^^^^^^^^^

Given :cpp:`n = std::distance(first,last)` and :cpp:`k = std::distance(piv_first,piv_last)`, exactly :math:`n \cdot log(k)` applications of the predicate and at most :math:`n \cdot log(k)` swaps.

Implementation analysis
^^^^^^^^^^^^^^^^^^^^^^^

The idea of the algorithm is very similar to quicksort where you partition by dichotomy:

   - first you partition by the middle pivot value, **piv_mid**, finding the middle partition point **pp_mid**
   - then you recursively partition each side

       - left side with **[first,pp_mid)** and **[piv_first,piv_mid)**
       - right side with **[pp_mid,last)** and **[piv_mid,piv_last)**

Complexity: :math:`n \cdot log(k)`

   - One each level :math:`lvl`, you have :math:`2^{lvl}` partitions to apply, and the union of these partitions iterates over the whole range (complexity n)
   - since you find :math:`2^{lvl}` partition points at each level, you need :math:`log(k)` levels

See also
^^^^^^^^

TODO: Pivot partitions points, indices, range versions


Example
^^^^^^^

.. literalinclude:: /../std_e/algorithm/test/pivot_partitions.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] pivot partitions {
  :end-before: [Sphinx Doc] pivot partitions }
