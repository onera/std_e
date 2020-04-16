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
