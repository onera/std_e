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
**lift.hpp**                      See :ref:`lift`. Convert a function template to a generic lambda
================================= =================================================================================

future/
=======
**future/** contains functionality that is one the way of being integrated to C++, or is already integrated but not always supported by the toolchains.

============================ =========================================================================================================
**span.hpp**                 Re-implementation of :cpp:`std::span`
**algorithm.hpp**            :cpp:`constexpr` standard algorithms
**constexpr_quick_sort.hpp** :cpp:`constexpr` :cpp:`std::sort`
**constexpr_vector.hpp**     Wrapper around :cpp:`std::array` with :cpp:`std::vector` semantics (:cpp:`push_back`, :cpp:`resize`...)
**contract.hpp**             Provides :cpp:`STD_E_ASSERT`, similar to :cpp:`assert`, but prints the stack trace
**zip.hpp**                  Provides the :cpp:`zip(Rng0& r0, Rng1& r1)` template function
============================ =========================================================================================================

utils/
======
=========================== ================================================================================================================
**array.hpp**               Common interface to create :cpp:`std::array` and :cpp:`std::vector` (needed in some generic contexts)
**concatenate.hpp**         Concatenate two ranges, append one range to another...
**enum.hpp**                See :ref:`enum`
**frozen_flat_map.hpp**     Key-value :cpp:`const` container implemented with two vectors
**functional.hpp**          :cpp:`identity` and :cpp:`identity_closure`
**pretty_print.hpp**        :cpp:`pretty_print_bytes`: human readable number of bytes (B, KB, MB...)
**string.hpp**              String utils: :cpp:`join`, :cpp:`split`, :cpp:`begins_with`, :cpp:`ends_with`
**switch.hpp**              See :ref:`switch`. Given a run-time integer :cpp:`i`, :cpp:`switch_` dispatches to a function overload which first argument is :cpp:`std::integral_constant<int,i>`
**to_string.hpp**           :cpp:`to_string` function for common types. :cpp:`range_to_string`: convert a range to a string
**tuple.hpp**               Tuple algorithms: :cpp:`for_each`, :cpp:`find_apply`, :cpp:`for_each_until`
**vector.hpp**              Vector algorithms: :cpp:`sort`, :cpp:`unique`, :cpp:`sort_unique`, :cpp:`iota`, :cpp:`to_string`,
=========================== ================================================================================================================


parallel/
=========
MPI functions wrappers

====================== ================================================================================================================
**mpi.hpp**            :cpp:`rank`, :cpp:`nb_ranks`, :cpp:`all_gather`, :cpp:`all_reduce`, :cpp:`[min|max|minmax]_global`...
**dist_graph.hpp**     Sparse communication pattern: :cpp:`dist_graph_create`, :cpp:`dist_graph_create_adj`
**all_to_all.hpp**     :cpp:`all_to_all`, :cpp:`all_to_all_v`, :cpp:`neighbor_all_to_all`, :cpp:`neighbor_all_to_all_v`...
**mpi_exception.hpp**  MPI exception class
**serialize.hpp**      Serialization/deserialization operations
====================== ================================================================================================================

operation/
==========
See :ref:`operation`. Represent mathematical operations by enumeration value, and bind them to their implementation

========================== ================================================================================================================
**operation_kind.hpp**     :cpp:`enum class operation_kind { plus, minus, ... , sqrt, cos, ..., tensor_prod, grad...}`
**operation_functor.hpp**  :cpp:`operation_functor<operation_kind>` to bind an operation name to its associated function
**dyn_op_functor.hpp**     :cpp:`apply_operation` and :cpp:`apply_operation_to_args`: dynamic dispatch from :cpp:`operation_kind` to the associated function
**logic.hpp**              :cpp:`implies`, :cpp:`equiv` and :cpp:`exclusive_or`
========================== ================================================================================================================


algorithm/
==========
=========================== ================================================================================================================
**algorithm.hpp**           :cpp:`equal_n`, :cpp:`accumulate_while_adjacent`, :cpp:`offset`, :cpp:`scale`
**iota.hpp**                Complements to :cpp:`std::iota`. Counted iota: :cpp:`iota_n`. Constant step iota: :cpp:`exclusive_iota`, :cpp:`exclusive_iota_n`, :cpp:`inclusive_iota`, :cpp:`inclusive_iota_n`
**distribution.hpp**        Create uniform distributions with :cpp:`uniform_distribution`
**for_each.hpp**            :cpp:`for_each_partition`, :cpp:`for_each_if`, :cpp:`replace_by_first_equivalent`
**id_permutations.hpp**     :cpp:`offset_permutation`
**permutation.hpp**         :cpp:`inverse_permutation`, :cpp:`inverse_partial_permutation`, :cpp:`compose_permutations`, :cpp:`sort_permutation`, :cpp:`unique_permutation`, :cpp:`permute`, :cpp:`zip_sort`
**iterator.hpp**            Transformations and predicates on iterator and indices: :cpp:`iterators_to_iterators`, :cpp:`iterators_to_indices`, :cpp:`indices_to_iterators`, :cpp:`equal_iterator_shifts`
**mismatch_points.hpp**     Operations when a mismatch appears in a sequence: :cpp:`for_each_mismatch`, :cpp:`mismatch_indices`
**numerics.hpp**            :cpp:`partial_accumulate`: equivalent to :cpp:`std::partial_sum`, but with an initial value, and one more element
**partition_sort.hpp**      :cpp:`partition_sort`: kind of partial sort. Given a range and a partition, sorts the range according to the partition
**sorting_networks.hpp**    :cpp:`sort_network`: see `Sorting networks on Wikipedia <https://en.wikipedia.org/wiki/Sorting_network>`_
**unique_compress.hpp**			:cpp:`unique_compress`: similar to :cpp:`std::unique`, but applies an operation to all unique elements instead of keeping the first one
=========================== ================================================================================================================


data_structure/
===============

=========================== ================================================================================================================
**heterogenous_range.hpp**  See :ref:`hvector`. Wrapper around a :cpp:`tuple<vector<Ts...>>`
**hierarchical_zip.hpp**    Zip together two or more :cpp:`structs` with the same data member names
**jagged_range.hpp**        See :ref:`jagged_array`. Similar to :cpp:`vector<vector<T>>`, but values are stored in a flat array, with an additional index array to keep track of the start and end of each inner array
**multi_range.hpp**         Multiple ranges zipped together into one
**table.hpp**               Spreadsheet-like class. Similar to :cpp:`multi_range`, but "column" names may be used to generate descriptive function names (through use of macros)
**strided_span.hpp**        :cpp:`span`-like class, with constant stride access (instead of contiguous)
=========================== ================================================================================================================

multi_index/
============
See :ref:`multi_array_highlights` and :ref:`multi_dimensional_arrays`. Multi-index are integer arrays used to represent indices in a multi-dimensional array.

=========================== ================================================================================================================
**multi_index.hpp**         Multi-index type :cpp:`multi_index<I,N>`. If :cpp:`N==-1` (default) the index rank is dynamic.
**multi_index_range.hpp**   Generators for multi-index iteration: :cpp:`fortran_multi_index_range`, :cpp:`c_multi_index_range` and :cpp:`multi_index_range_with_order`
=========================== ================================================================================================================

multi_array/
============
See :ref:`multi_array_highlights` and :ref:`multi_dimensional_arrays`

=========================== ================================================================================================================
**multi_array.hpp**         :cpp:`multi_array<Range,Shape>`: generic type for multi_arrays. :cpp:`make_sub_array`: make sub-array with contiguous memory
**multi_array_types.hpp**   :cpp:`fixed_multi_array`, :cpp:`dyn_multi_array` and :cpp:`dyn_multi_array_view`
**strided_array.hpp**       :cpp:`column`, :cpp:`row` and :cpp:`make_strided_array`
**make_block_view.hpp**     :cpp:`make_block_view`
=========================== ================================================================================================================

graph/
======
See :ref:`graph`

=========================== ================================================================================================================
**nested_tree/**            See :ref:`nested_tree`. :cpp:`nested_tree`: type for handling trees.
**adjacency_graph/**        See :ref:`adjacency_graph`. Adjacency graph types, notably :cpp:`adjacency_graph` and :cpp:`io_adjacency_graph`
**algorithm/**              Depth-first search (DFS) graph traversal algorithm: depth-first find (stop the traversal, )
**build/**
=========================== ================================================================================================================

interval/
=========
See :ref:`interval`. Interval, multi-interval and interval sequence operations.

=========================== ================================================================================================================
**interval.hpp**            :cpp:`interval<I>` and :cpp:`closed_interval<I>`
**algorithm.hpp**           Interval algorithms :cpp:`is_interval`, :cpp:`rotated_position`
**multi_interval.hpp**      :cpp:`multi_interval<I>`
**interval_sequence.hpp**   :cpp:`interval_sequence<Range>`, :cpp:`interval_vector<I>`, :cpp:`interval_span<I>`, :cpp:`interval_lengths`, :cpp:`indices_from_strides`, :cpp:`interval_index`
=========================== ================================================================================================================

iterator/
=========
=========================== ================================================================================================================
**itertor_range.hpp**       :cpp:`iterator_range`: view a pair of iterators as a range
=========================== ================================================================================================================


unit_test
=========
Unit test utilities with doctest.
