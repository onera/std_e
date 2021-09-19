#pragma once


#include "std_e/parallel/mpi/one_sided/window.hpp"


namespace std_e {


class window_guard {
  private:
    MPI_Win win;
    MPI_Comm comm;
  public:
    template<class T>
    window_guard(const window<T>& w)
      : win(w.underlying())
      , comm(w.communicator())
    {
      int assertion = 0;
      int err = MPI_Win_lock_all(assertion,win);
      STD_E_ASSERT(!err);
    }

    ~window_guard() {
      int err = MPI_Win_unlock_all(win);
      assert(!err);
      MPI_Barrier(comm);
    }
};


} // std_e
