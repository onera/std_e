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
      MPI_Win_lock_all(assertion,win);
    }

    ~window_guard() {
      MPI_Win_unlock_all(win);
      MPI_Barrier(comm);
    }
};


} // std_e
