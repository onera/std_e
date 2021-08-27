#pragma once


#include "std_e/parallel/mpi/base.hpp"


namespace std_e {


template<class T>
class window {
  private:
    MPI_Aint dn_elt;
    MPI_Win win;
    T* ptr;
  public:
    window(MPI_Aint dn_elt, MPI_Comm comm)
      : dn_elt(dn_elt)
    {
      int type_sz = sizeof(T);
      MPI_Win_allocate(dn_elt*type_sz,type_sz,MPI_INFO_NULL,comm,&ptr,&win);
    }
    ~window() {
      MPI_Win_free(&win);
    }

    auto local()       { return make_span<      T>(ptr,dn_elt); }
    auto local() const { return make_span<const T>(ptr,dn_elt); }

    auto
    underlying() const {
      return win;
    }
};


class win_lock_all_guard {
  private:
    MPI_Win win;
  public:
    template<class T>
    win_lock_all_guard(const window<T>& w)
      : win(w.underlying())
    {
      int assertion = 0;
      MPI_Win_lock_all(assertion,win);
    }

    ~win_lock_all_guard() {
      MPI_Win_unlock_all(win);
    }
};


auto
check_unified_memory_model(MPI_Win win) {
  int* win_mem_model;
  int flag;
  MPI_Win_get_attr(win,MPI_WIN_MODEL,&win_mem_model,&flag);
  STD_E_ASSERT(flag);
  if (*win_mem_model!=MPI_WIN_UNIFIED) {
    throw mpi_exception(-1,"require MPI_WIN_UNIFIED memory model");
  }
}


} // std_e
