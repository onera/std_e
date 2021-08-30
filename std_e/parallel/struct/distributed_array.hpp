#pragma once


#include "std_e/parallel/mpi/one_sided/window.hpp"


namespace std_e {


template<class Integer> using distribution_vector = std_e::interval_vector<Integer>;

template<class T, class Distribution = distribution_vector<g_num>>
class distributed_array {
    // invariants:
    //   - handle refers to an object which is valid over comm
    //   - distri is of size n_rank+1
    //   - local_block.size() == length(distri,i_rank)
  private:
    MPI_Comm comm;
    Distribution distri;
    window<T> w;
  public:
    using value_type = T;

    distributed_array(Distribution distri, MPI_Comm comm)
      : comm(comm)
      , distri(std::move(distri))
      , w(size(),comm)
    {
      check_unified_memory_model(w.underlying());
    }

    auto local()       { return w.local(); }
    auto local() const { return w.local(); }

    auto total_size() const {
      return length(distri);
    }
    auto size() const {
      return distri.length(rank(comm));
    }

    auto
    distribution() const -> const auto& {
      return distri;
    }

    auto win()       ->       auto& { return w; }
    auto win() const -> const auto& { return w; }
};


} // std_e
