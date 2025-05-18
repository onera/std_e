.. _md_array_dim_type :

Multi-dimensional array index and dimension types
=================================================

To fix ideas. Let `a` be a multi-dimensional array of dimensions (m,n). What should be the type of the dimensions (m and n)?
Lets now index into `a`: a(i,j). What should be the type of the indices (i and j).

Reminder: for computing a(i,j), we need to compute a linear index `l`. For example, assuming a Fortran order, we would have: `l = i*m + j`

Questions
---------
1. What should be the dimensions type of a multi-dimensional array?
2. What should be the index type of a multi-dimensional-array?
3. What is the link between both and how do they relate to the multi-dimensiontal array type?

Answers
-------

Dimension type
""""""""""""""

The consensus is to use signed integers, so we are left with int32 and int64. For runtime-sized array, the dimensions are stored in the multi-dimensional array, so the array class needs to know the type: either we make the dimension type a template parameter or fix it to either int32 or int64. Since we do not see any use-case where using int64 for dimensions would introduce overhead (see below), we would like to fix int64. One thing that could prevent us from doing so is if we want to have the dimension type and the index type to be the same. But we'll see that we don't have a compelling reason to do that. So the dimensions type should be int64.

Index type
""""""""""

  For performance (memory space and linear index computation), we want the index type to be sometimes int32, and sometimes int64. 

  Assumption: the relative cost of computation of linear index computation (such as `l = i*m + j`) might matter on some circonstances, and int32 computations are faster than int64 computations. So using int32 indices and int32 dimensions **is** more performant that using int64 indices and int64 dimensions. Maybe 99% of the time it does not matter, but I would still not find it satisfing to make this bet, especially because the user may only have stored int32 indices for memory space.

  Proposal: make the multi-dimensional array class support any index type. It means: indexing methods (such as `operator(Integer...)`) should be templated. Note however that the type itself does not need to be templated.

Link between index and dimension types
""""""""""""""""""""""""""""""""""""""

  In practice, we may want an index type that is smaller or equal to the dimension type (we don't see any use of indexing with int64 into arrays whose dimensions are int32).

  Making the index type be exactly the dimension type would have the unwanted consequence of needing to parametrize the multi-array type by the dimensions type (can't use int64 only anymore because indices required to by int64 could be detrimental to performance in some cases).

  We are left with int32 indices and int64 dims. Should we allow it? Performance-wise, linear-index computation can be done in the index type (i.e. truncate int64 dims to int32 if indices are int32). It is unlikely that the truncation causes any overhead (regarding hardware, it is most likely a no-op, and even if something needs to be done, it will most likely be done once, before entering a linear-index computation loop. So no impact performance-wise. Then regarding safety, we could overflow dimensions from int64 to int32, but considering that indices are supposed to be less that dimensions, it means that the indices either overflow in int32, or that some parts of the array an inaccessible. So it seems that an assertion, or taking the risk in Release mode, is fine.
