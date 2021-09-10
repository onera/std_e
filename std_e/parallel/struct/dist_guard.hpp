#pragma once


#include "std_e/parallel/mpi/one_sided/window_guard.hpp"


namespace std_e {


class dist_guard {
  public:
    template<class dist_array_type>
    dist_guard(const dist_array_type& a)
      : g(a.win())
    {}
  private:
    window_guard g;
};


} // std_e
