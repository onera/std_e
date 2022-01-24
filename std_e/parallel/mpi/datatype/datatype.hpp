#pragma once


#include <mpi.h>
#include <cassert>
#include "std_e/future/contract.hpp"


namespace std_e {


class mpi_data_type {
  private:
    MPI_Datatype mpi_type;
  public:
    mpi_data_type()
      : mpi_type(MPI_DATATYPE_NULL)
    {}

    mpi_data_type(MPI_Datatype mpi_type)
      : mpi_type(mpi_type)
    {
      int err = MPI_Type_commit(&mpi_type);
      STD_E_ASSERT(!err);
    }

    mpi_data_type(const mpi_data_type&) = delete;
    mpi_data_type& operator=(const mpi_data_type&) = delete;
    mpi_data_type(mpi_data_type&& x)
      : mpi_type(x.mpi_type)
    {
      x.mpi_type = MPI_DATATYPE_NULL;
    }
    mpi_data_type& operator=(mpi_data_type&& x) {
      mpi_type = x.mpi_type;
      x.mpi_type = MPI_DATATYPE_NULL;
      return *this;
    }

    ~mpi_data_type() {
      if (mpi_type!=MPI_DATATYPE_NULL) {
        [[maybe_unused]] int err = MPI_Type_free(&mpi_type);
        STD_E_ASSERT_ABORT(!err);
      }
    }

    auto
    underlying() const -> MPI_Datatype {
      return mpi_type;
    }
};


template<class Int_range> auto
indexed_block(const Int_range& ins, int type_sz) {
  auto n = ins.size();

  MPI_Datatype mpi_type;

  int err = MPI_Type_create_indexed_block(n, 1, ins.data(), to_mpi_type_of_size(type_sz), &mpi_type);
  STD_E_ASSERT(!err);

  return mpi_data_type(mpi_type);
}

// same as indexed block but variable stride
template<class Int_range, class Int_contiguous_range> auto
indexed(const Int_range& ins, const Int_contiguous_range& strides, const Int_contiguous_range& displs, int type_sz) {
  auto n = ins.size();

  MPI_Datatype mpi_type;
  int err = MPI_Type_indexed(n, strides.data(), displs.data(), to_mpi_type_of_size(type_sz), &mpi_type);
  STD_E_ASSERT(!err);

  return mpi_data_type(mpi_type);
}


} // std_e
