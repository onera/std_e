.. _jagged_array:

Jagged array
============

Jagged_vector
-------------

Suppose we want to store arrays :cpp:`[9,5,6,7]`, :cpp:`[1,3]` and :cpp:`[8,2,4]`. Since their size is different, we can't use a multi-dimensional array. The default choice is to use :cpp:`vector<vector<int>>`. However, in some contexts, we may want to use a :cpp:`jagged_vector<int>` instead. A :cpp:`jagged_vector` is a data structure internally holding two vectors.

* The first vector stores the concatenated data of all the arrays. In our example, it would hold :cpp:`[9,5,6,7,1,3,8,2,4]`. It can be accessed with the :cpp:`.flat_view()` method.
*  The second vector hold the indices of the intervals where the inner arrays start and stop. In our example, it would be :cpp:`[0,4,6,9]`. The type of the vector is actually a :ref:`interval_vector <interval_seq>`, it can be accessed with the :cpp:`.indices()` method.

The :cpp:`jagged_vector` interface tries to mimic the one of :cpp:`std::vector` where it makes sense. Example:

.. literalinclude:: /../std_e/data_structure/test/jagged_range.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] jagged_vector {
  :end-before: [Sphinx Doc] jagged_vector }


Generalizations
---------------

Jagged_range
^^^^^^^^^^^^
It may be desirable to use another range type than :cpp:`std::vector` to store the arrays. It is then possible to use the more general template :cpp:`jagged_range`. A particular case is :cpp:`jagged_span`, which is similar to :cpp:`jagged_vector`, except it does not own its values.

Jagged arrays of higher dimensions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
The full declaration of a :cpp:`jagged_vector` is actually :cpp:`jagged_vector<T,N>`, where :cpp:`N` is the rank of the array. It defaults to two, as we have seen, but it can be used with higher ranks. It can also be used with :cpp:`N==1`, where it is a type similar to :cpp:`std::vector`.

Multi-jagged arrays
^^^^^^^^^^^^^^^^^^^
If multiple vectors have to be zipped together (similar to what is done with a :cpp:`multi_vector`), a :cpp:`jagged_multi_vector<2,Ts...>` can be used.

