# Multi-dimensional arrays #

## Introduction ##
There are many sources of variability regarding multi-dimensional arrays:
 * Type of the values stored
 * Memory allocation of the values
 * Shapes of the array
    * rank
    * dimensions
    * offsets
    * dynamic or fixed sizes for all of these
 * Operations supported
 * Memory orders: Fortran/column-major, C/row-major, hybrid orders
 * Views and subviews, strides
 * Conversions
 * Supported operations

## Concepts and base class ##
[Note: we use "base" in the sense of "fundational", NOT inheritance]
In `std_e`, we define three core concepts, `Multi_array`, `Memory_ressource` and `Multi_array_shape`.

We propose a base class template, `multi_array`, modeling the `Multi_array` concept, that is parametrized by what we think is the core basic functionnality, namely, a `Memory_ressource` and a `Multi_array_shape`.
 * parametrizing by a `Memory_ressource` allows to parametrize according to a value type and a memory allocation strategy
 * parametrizing by a `Multi_array_shape` allows to parametrize according to the shape
    * a `Multi_array_shape` can have fixed or dynamic dimensions, in the latter case, the rank may be dynamic
    * ranks can be >=0 (in particular, we are not restricted to 2D arrays)
Other parameters are fixed:
 * the memory is supposed to be contiguous and Fortran-ordered (see below for alternatives)
 * there is always an offset (0 by default), it needs to be stored if the dimensions are dynamic (even if not used)
 * `multi_array` does not support strides/non-contiguous memory (see below for alternatives)

## Usage ##
`multi_array` is not very friendly to use. More convenient multi-dimensional arrays are defined from it:
 * `fixed_multi_array<T,Dim0,Dim1,...>>`
    * multi-array of type T
    * fixed rank and dimensions
    * allocated on the stack
 * `dyn_multi_array<T,Index_type,rank>` 
    * type T
    * dynamic dimensions, and fixed of dynamic rank (if `rank==dynamic_size`)
    * owned memory (uses a `std::vector<T>`)
    * `Index_type` must be chosen according to the size requirements (int32 or int64)
 * `dyn_multi_array_view`
    * same as `dyn_multi_array`...
    * ...but the `Memory_ressource` is a `memory_view` (i.e. a pointer to external memory)
    * WARNING: even if `dyn_multi_array_view` behaves like a view regarding values array coefficients, it is not a "pure" view in the sense that its shape holds the dimensions. Hence, creating a `dyn_multi_array_view` is cheap, but NOT free. Plus, if created from another `Multi_array`, modifying dimensions of one of them will NOT modify the dimensions of the other.
      * maybe the "view" word should have been avoided. But it seems to be the natural way to use external memory multi-arrays, because the memory of the coefficients is really a (critical) ressource, and it makes sense to manage it carefully, whereas it is not the case for the dimensions themselves (a few integers)
      * the main reason `dyn_multi_array_view` is not a "pure" view is to be able to create a view from external memory (regarding the coefficients), without needing to refer to exteral memory for its shape (this would imply complex memory management if both external memories are not related)

## Other classes TODO ##
Strided multi-dimensional array views can be used with `smulti_array_view`. Owning not contiguous memory doesn't really make sense, so strided multi-array do not owned memory.

## Miscellaneous ##
 * There is no implicit conversion between `multi_array` classes. To create a view, use `make_view`.
 * C-order is just Fortran-order with reversed indices, so use of C-order can be done by wrapping `multi_array` into a class where accessors do reverse the indices. Same for other "hybrid" orders: just permute the indices.
