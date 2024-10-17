.. _blocks:

Blocks
======

Introduction
------------

TODO linked to deprecated `jagged_array`, so report jagged doc here

Vocabulary
----------

We often want to store and manipulate sequences of arrays of different sizes:
- graphs
- meshs

The data is given by two arrays:
- A :def:`value array`, or just :def:`array`. For example `arr = {9,5,6,7,1,3,8,2,4}`
- An :def:`index array`, or just :def:`indices`: For example `arr_idx = {0,4,6,9}`

The indices give boundaries of sub-arrays. Here, we have:
- sub-array 0 is `{9,5,6,7}`. It begins at index 0 and ends at index 4.
- sub-array 1 is `{1,3}`. It begins at index 4 and ends at index 6.
- sub-array 1 is `{8,2,4}`. It begins at index 6 and ends at index 9.

A more visual way to represent the data, that we use extensively thoughout the unit tests, is the following:

.. code-block:: C++

   arr_idx = {0      ,  4  ,  6    ,  9}
   arr     = {9,5,6,7,  1,3,  8,2,4}

A sub-array is called a :def:`block`.

We can easily deduce the block sizes from the indices:
