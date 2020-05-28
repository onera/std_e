Value, reference and view types
===============================
    behaves like a view regarding values array coefficients, it is not a "pure" view in the sense that its shape holds the dimensions. Hence, creating a `dyn_multi_array_view` is cheap, but NOT free. Plus, if created from another `Multi_array`, modifying dimensions of one of them will NOT modify the dimensions of the other.

    * maybe the "view" word should have been avoided. But it seems to be the natural way to use external memory multi-arrays, because the memory of the coefficients is really a (critical) ressource, and it makes sense to manage it carefully, whereas it is not the case for the dimensions themselves (a few integers)
    * the main reason `dyn_multi_array_view` is not a "pure" view is to be able to create a view from external memory (regarding the coefficients), without needing to refer to exteral memory for its shape (this would imply complex memory management if both external memories are not related)

Reference
    // NOTE 0: 
    //   be it either copy or move,
    //   assigning the reference means assigning underlying values
    //   and this must be done by std::copy (not std::move), because
    //   the refered values are not owned
    //   (hence the reference is not allowed to move them, even if it is a rvalue reference)
    // NOTE 1:
    //   copy ctor and copy assignement do not have the same semantics
    //     * the ctor creates a reference by creating a reference location
    //     * the assignment assigns new values to the viewed values
    //   same for move ctor and move assignement
