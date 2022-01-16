.. _pivot_partitions_range:

Pivot partitions - range versions
---------------------------------


(1)  .. code-block:: c++

       template<class RA_rng, class RA_pivot_rng>
         pivot_partition(RA_rng& x, const RA_pivot_rng& pivots)
          -> Return_container

(2)  .. code-block:: c++

       template<class RA_rng, class RA_pivot_rng,
                class Bin_pred>
         pivot_partition(RA_rng& x, const RA_pivot_rng& pivots,
                         Bin_pred comp)
          -> Return_container

(3)  .. code-block:: c++

       template<class RA_rng, class RA_pivot_rng,
                class Bin_pred,
                class Return_container>
         pivot_partition(RA_rng& x, const RA_pivot_rng& pivots,
                         Bin_pred comp,
                         Return_container&& partition_pts)
          -> Return_container

(4)  .. code-block:: c++

       template<class RA_rng, class RA_pivot_rng>
       pivot_partition_point(RA_rng& x, const RA_pivot_rng& pivots)
          -> Return_container

(5)  .. code-block:: c++

       template<class RA_rng, class RA_pivot_rng,
                class Bin_pred>
       pivot_partition_point(RA_rng& x, const RA_pivot_rng& pivots,
                             Bin_pred comp)
          -> Return_container

(6)  .. code-block:: c++

       template<class RA_rng, class RA_pivot_rng,
                class Bin_pred,
                class Return_container>
       pivot_partition_point(RA_rng& x, const RA_pivot_rng& pivots,
                             Bin_pred comp,
                             Return_container&& partition_pts)
          -> Return_container

(7)  .. code-block:: c++

       template<class RA_rng, class RA_pivot_rng>
         pivot_partition_indices(RA_rng& x, const RA_pivot_rng& pivots)
          -> std_e::interval_vector<int>

(8)  .. code-block:: c++

       template<class RA_rng, class RA_pivot_rng,
                class Bin_pred>
         pivot_partition_indices(RA_rng& x, const RA_pivot_rng& pivots,
                                 Bin_pred comp)
          -> std_e::interval_vector<int>

(9)  .. code-block:: c++

       template<class RA_rng, class RA_pivot_rng,
                class Bin_pred,
                class Return_container>
         pivot_partition_indices(RA_rng& x, const RA_pivot_rng& pivots,
                                 Bin_pred comp,
                                 Return_container&& partition_pts)
          -> Return_container

(10)  .. code-block:: c++

       template<class RA_rng, class RA_pivot_rng>
         pivot_partition_point(RA_rng& x, const RA_pivot_rng& pivots)
          -> std_e::interval_vector<int>

(11)  .. code-block:: c++

       template<class RA_rng, class RA_pivot_rng,
                class Bin_pred>
         pivot_partition_point_indices(RA_rng& x, const RA_pivot_rng& pivots,
                                       Bin_pred comp)
          -> std_e::interval_vector<int>

(12)  .. code-block:: c++

       template<class RA_rng, class RA_pivot_rng,
                class Bin_pred,
                class Return_container>
         pivot_partition_point_indices(RA_rng& x, const RA_pivot_rng& pivots,
                                       Bin_pred comp,
                                       Return_container&& partition_pts)
          -> Return_container




(1) Partitions the range **x** according to **pivots**.

    This means that all the elements of **x** smaller than :cpp:`pivots[0]` will come first, then all the elements smaller than :cpp:`pivots[1]` will come second... etc.

    Given :cpp:`k = std::size(pivots)`, range **x** is then partitioned into k+1 buckets of ever greater elements.

    Each partition point, that is, each iterator separating the buckets, is written to the returned container. The type :cpp:`Return_container` is :cpp:`std_e::interval_vector<RA_rng::iterator>`.

    The partition is not stable: the order of equal elements is not guaranteed to be preserved.

(2) Same as (1) but use of function **comp** to compare elements of the range to the pivots.

    **comp** is a binary predicate. Its first argument is supposed to be an element in the range **x**, and its second argument is supposed to be an element of **pivots**.

(3) Same as (2) but use container **partition_pts** as the output result. The container is resized and its interest is mainly to provide a different type than the one of versions (1) and (2). **partition_pts** must be passed by rvalue reference.

(4,5,6) Same as (1,2,3) respectively, but presupposes **x** has been partitioned as of :cpp:`std_e::pivot_partition`, and simply returns the partition points.

(7,8) Same as (1,2) respectively, but returns a :cpp:`std_e::interval_vector<int>` of partition **indices** (instead of the partition iterators).

(9) Same as (3), but returns a container of partition **indices** (instead of the partition iterators).

(10,11) Same as (4,5) respectively, but returns a :cpp:`std_e::interval_vector<int>` of partition **indices** (instead of the partition iterators).

(12) Same as (6), but returns a container of partition **indices** (instead of the partition iterators).


Parameters
^^^^^^^^^^

x
  the range of elements to partition

pivots
  the range of pivots that will be used to partition **x**

  **pivots** must be sorted according to **comp**.

comp
  comparison function object (i.e. an object that satisfies the requirements of Compare) which returns true if the first argument is less than (i.e. is ordered before) the second.

partition_pts (3,6,9,12)
  container where the partition points (3,6) or the partition indices will be written

  must be passed be rvalue reference

Type requirements
^^^^^^^^^^^^^^^^^

RA_rng, RA_pivot_rng
  must meet the requirements of a RandomAccessRange.


Bin_pred
  The signature of the comparison function should be equivalent to the following:

.. code-block:: c++

  bool comp(const T0& x, const T1& y);

where :cpp:`using T0 = RA_it::value_type` and :cpp:`using T1 = RA_it_piv::value_type`

Return value
^^^^^^^^^^^^

Container of size k+2. The first and last elements are only given for convenience and are
 - :cpp:`begin(x)` and :cpp:`end(x)` in versions (1-6)
 - :cpp:`0` and :cpp:`x.size()` in versions (7-12)

The elements in-between give the partition points (1-6) or partition indicies (7-12) which delimit the partitions of **x** according to **pivots**.

The container is of type:
 - :cpp:`std_e::interval_vector<RA_rng::iterator>` (1,2,4,5)
 - :cpp:`Return_container`, specified by the user (3,6). The equality :cpp:`Return_container::value_type == RA_rng::iterator` must hold.
 - :cpp:`std_e::interval_vector<int>` (7,8,10,11)
 - :cpp:`Return_container`, specified by the user (9,12). :cpp:`Return_container::value_type` must be an integer type, and able to hold the value :cpp:`x.size()`.

Complexity
^^^^^^^^^^

See :ref:`pivot_partitions`.

Implementation analysis
^^^^^^^^^^^^^^^^^^^^^^^

See :ref:`pivot_partitions`.

See also
^^^^^^^^

:ref:`pivot_partitions`: iterator versions of these algorithms

Examples
^^^^^^^^

1.

.. literalinclude:: /../std_e/algorithm/test/pivot_partition_range.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] pivot_partition range {
  :end-before: [Sphinx Doc] pivot_partition range }

4.

.. literalinclude:: /../std_e/algorithm/test/pivot_partition_range.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] pivot_partition_point range {
  :end-before: [Sphinx Doc] pivot_partition_point range }

7.

.. literalinclude:: /../std_e/algorithm/test/pivot_partition_range.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] pivot_partition_indices range {
  :end-before: [Sphinx Doc] pivot_partition_indices range }

8.

.. literalinclude:: /../std_e/algorithm/test/pivot_partition_range.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] pivot_partition_indices comp range {
  :end-before: [Sphinx Doc] pivot_partition_indices comp range }

9.

.. literalinclude:: /../std_e/algorithm/test/pivot_partition_range.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] pivot_partition_indices ret range {
  :end-before: [Sphinx Doc] pivot_partition_indices ret range }

10.

.. literalinclude:: /../std_e/algorithm/test/pivot_partition_range.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] pivot_partition_point_indices range {
  :end-before: [Sphinx Doc] pivot_partition_point_indices range }
