#pragma once


namespace std_e {


class mpi_data_type {
  private:
    MPI_Datatype mpi_type;
  public:
    mpi_data_type(MPI_Datatype mpi_type)
      : mpi_type(mpi_type)
    {
      int err = MPI_Type_commit(&mpi_type);
      STD_E_ASSERT(!err);
    }

    ~mpi_data_type() {
      int err = MPI_Type_free(&mpi_type);
      assert(!err);
    }

    auto
    underlying() -> MPI_Datatype {
      return mpi_type;
    }
};


template<class T, class Int_range> auto
indexed_block(const Int_range& ids) {
  auto n = ids.size();

  MPI_Datatype mpi_type;
  int err = MPI_Type_create_indexed_block(n, 1, ids.data(), to_mpi_type<T>, &mpi_type);
  STD_E_ASSERT(!err);

  return mpi_data_type(mpi_type);
}


} // std_e
