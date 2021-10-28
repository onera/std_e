#pragma once


#include "std_e/parallel/mpi/one_sided/window.hpp"
#include "std_e/parallel/mpi/datatype/datatype.hpp"


namespace std_e {


template<class T, class I> auto
_get_contiguous(MPI_Win win, int rank, MPI_Aint disp, I size, T* out) -> void {
  int err = MPI_Get(
    out      ,size,to_mpi_type<T>, // origin args
    rank,disp,size,to_mpi_type<T>, // target args
    win
  );
  STD_E_ASSERT(!err);
}

template<class T, class I> auto
_get_indexed(MPI_Win win, int rank, MPI_Datatype target_type, I size, T* out) -> void {
  int err = MPI_Get(
    out    ,size, to_mpi_type<T>, // origin args
    rank, 0,   1, target_type   , // target args
    win
  );
  STD_E_ASSERT(!err);
}


template<class T> auto
get_scalar(const window<T>& win, int rank, MPI_Aint disp, T& out) -> void {
  _get_contiguous(win.underlying(),rank,disp,1,&out);
}

template<class T, class Range> auto
get_contiguous(const window<T>& win, int rank, MPI_Aint disp, Range& out) -> void {
  _get_contiguous(win.underlying(),rank,disp,out.size(),out.data());
}


class protocol_win_get_indexed {
  private:
    int type_sz;
    int n_elt;
    mpi_data_type target_type;
  public:
    protocol_win_get_indexed() = default;

    template<class Int_range>
    protocol_win_get_indexed(const Int_range& ins, int type_sz)
      : type_sz(type_sz)
      , n_elt(ins.size())
      , target_type(indexed_block(ins,type_sz))
    {}

    auto
    number_of_elements() const -> int {
      return n_elt;
    }

    template<class T, class Range> auto
    request(const window<T>& win, int rank, Range& out) const -> void {
      STD_E_ASSERT(sizeof(T)==type_sz);
      STD_E_ASSERT((int)out.size()==n_elt);
      _get_indexed(win.underlying(),rank,target_type.underlying(),n_elt,out.data());
    }
};


//template<class T>
//class protocol_win_get_indexed_v {
//  private:
//    mpi_data_type target_type;
//  public:
//    template<class TR, class IR>
//    protocol_win_get_indexed(const jagged_range<TR,IR,2>& ins)
//      : target_type(indexed<T>(ins))
//    {}
//
//    template<class Range> auto
//    request(const window<T>& win, int rank, Range& out) -> void {
//      _get_indexed(win.underlying(),rank,target_type.underlying(),out.size(),out.data());
//    }
//};



//template<class T> auto
//rget(const window<T>& win, int rank, MPI_Aint disp, T& out) -> MPI_Request {
//  MPI_Request req;
//  MPI_Rget(
//    &out     ,1,to_mpi_type<T>, // origin args
//    rank,disp,1,to_mpi_type<T>, // target args
//    win.underlying(),&req
//  );
//  return req;
//}



} // std_e
