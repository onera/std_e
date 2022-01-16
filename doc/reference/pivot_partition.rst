.. _pivot_partitions:

Pivot partitions
----------------


(1)  .. code-block:: c++

       template<class RA_it, class RA_it_piv, class RA_it_out>
         pivot_partition(RA_it first, RA_it last,
                         RA_it_piv piv_first, RA_it_piv piv_last,
                         RA_it_out out_first)

(2)  .. code-block:: c++

       template<class RA_it, class RA_it_piv, class RA_it_out,
                class Bin_pred>
         pivot_partition(RA_it first, RA_it last,
                         RA_it_piv piv_first, RA_it_piv piv_last,
                         RA_it_out out_first,
                         Bin_pred comp)

(3)  .. code-block:: c++

       template<class RA_it, class RA_it_piv, class RA_it_out>
         pivot_partition_point(RA_it first, RA_it last,
                               RA_it_piv piv_first, RA_it_piv piv_last,
                               RA_it_out out_first)

(4)  .. code-block:: c++

       template<class RA_it, class RA_it_piv, class RA_it_out,
                class Bin_pred>
         pivot_partition_point(RA_it first, RA_it last,
                               RA_it_piv piv_first, RA_it_piv piv_last,
                               RA_it_out out_first,
                               Bin_pred comp)

(5)  .. code-block:: c++

       template<class RA_it, class RA_it_piv, class RA_it_out>
         pivot_partition_indices(RA_it first, RA_it last,
                                 RA_it_piv piv_first, RA_it_piv piv_last,
                                 RA_it_out out_first)

(6)  .. code-block:: c++

       template<class RA_it, class RA_it_piv, class RA_it_out,
                class Bin_pred>
         pivot_partition_indices(RA_it first, RA_it last,
                                 RA_it_piv piv_first, RA_it_piv piv_last,
                                 RA_it_out out_first,
                                 Bin_pred comp)

(7)  .. code-block:: c++

       template<class RA_it, class RA_it_piv, class RA_it_out>
         pivot_partition_point_indices(RA_it first, RA_it last,
                                       RA_it_piv piv_first, RA_it_piv piv_last,
                                       RA_it_out out_first)

(8)  .. code-block:: c++

       template<class RA_it, class RA_it_piv, class RA_it_out,
                class Bin_pred>
         pivot_partition_point_indices(RA_it first, RA_it last,
                                       RA_it_piv piv_first, RA_it_piv piv_last,
                                       RA_it_out out_first,
                                       Bin_pred comp)


(1) Partitions the range [first,last) according to the pivots [piv_first,piv_last).

    This means that all the elements of [first,last) smaller than :cpp:`*piv_first` will come first, then all the elements smaller than :cpp:`*(piv_first+1)` will come second... etc.

    Given :cpp:`k = std::distance(piv_first,piv_last)`, range [first,last) is then partitioned into k+1 buckets of ever greater elements.

    Each partition point, that is, each iterator separating the buckets, is written to the output range [out_first,k).

    The partition is not stable: the order of equal elements is not guaranteed to be preserved.

(2) Same as (1) but use of function **comp** to compare elements of the range to the pivots.

    **comp** is a binary predicate. Its first argument is supposed to be an element in the range [first,last), and its second argument is supposed to be an element in the range of pivots [piv_first,piv_last).


(3) Presupposes [first,last) has been partitioned as of :cpp:`std_e::pivot_partition`, and simply returns the partition points in [out_first,k).

(4) Same as (3) but with user-provided comparison function.

(5) Same as (1) but the partition **indices** (instead of the partition iterators) in the range [first,last) are returned in [out_first,k).

(6) Same as (5) but with user-provided comparison function.

(7) Same as (2) but the partition **indices** (instead of the partition iterators) in the range [first,last) are returned in [out_first,k).

(8) Same as (7) but with user-provided comparison function.

Parameters
^^^^^^^^^^

first, last
  the range of elements to partition

piv_first, piv_last
  the range of pivots that will be used to partition [first,last)

  [piv_first, piv_last) must be sorted according to **comp**.

out_first
  the beginning of the destination range

  the range [out_first,k) is supposed to be valid

comp
  comparison function object (i.e. an object that satisfies the requirements of Compare) which returns true if the first argument is less than (i.e. is ordered before) the second.


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

  Versions (1,2,3,4)
    :cpp:`RA_it_out::value_type` should be the same as :cpp:`RA_it`

  Versions (5,6,7,8)
    :cpp:`RA_it_out::value_type` should be in integer type that can hold at least the value :cpp:`std::distance(first,last)`.

Return value
^^^^^^^^^^^^

(none)

Complexity
^^^^^^^^^^

Versions (1,2,3,4)
  Given :cpp:`n = std::distance(first,last)` and :cpp:`k = std::distance(piv_first,piv_last)`, at most :math:`n / 2 \cdot log(k)` applications of the predicate and :math:`n / 2 \cdot log(k)` swaps.

Versions (5,6,7,8)
  At most :math:`k \cdot log(n/k) \cdot log(k)`

Implementation analysis
^^^^^^^^^^^^^^^^^^^^^^^

The idea of the algorithm is very similar to quicksort where you partition by dichotomy:

   - first you partition by the middle pivot value, **piv_mid**, finding the middle partition point **pp_mid**
   - then you recursively partition each side

       - left side with **[first,pp_mid)** and **[piv_first,piv_mid)**
       - right side with **[pp_mid,last)** and **[piv_mid,piv_last)**

Complexity: :math:`n / 2 \cdot log(k)`

   - One each level :math:`lvl`, we have :math:`2^{lvl}` partitions to apply, and the union of these partitions iterates over the whole range (complexity n).
   - Since we find :math:`2^{lvl}` partition points at each level, we need :math:`log(k)` levels.
   - Actually complexity n means n/2 comparisons and n/2 potential swaps since we iterate from both ends alltogether.

See also
^^^^^^^^

:ref:`pivot_partitions_range`: Higher-level, range versions of these algorithms

Examples
^^^^^^^^

.. literalinclude:: /../std_e/algorithm/test/pivot_partition.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] pivot_partition {
  :end-before: [Sphinx Doc] pivot_partition }

.. literalinclude:: /../std_e/algorithm/test/pivot_partition.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] pivot_partition_point {
  :end-before: [Sphinx Doc] pivot_partition_point }

.. literalinclude:: /../std_e/algorithm/test/pivot_partition.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] pivot_partition_indices {
  :end-before: [Sphinx Doc] pivot_partition_indices }

.. literalinclude:: /../std_e/algorithm/test/pivot_partition.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] pivot_partition_point_indices {
  :end-before: [Sphinx Doc] pivot_partition_point_indices }
