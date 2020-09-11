Permutation
===========

Warning: sort_permutation actually returns the inverse sort permutation (it returns new-to-old positions)
This is done because the algorithm directly returns the inverse, and getting the direct (old-to-new) is an extra step that may not be useful.

In order to interoperate well with sort_permutation, permute_copy takes an inverse permutation to permute its range.
