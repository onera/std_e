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
    window<T> wind;
  public:
    using value_type = T;

    distributed_array(Distribution distri, MPI_Comm comm)
      : comm(comm)
      , distri(std::move(distri))
      , wind(size(),comm)
    {
      check_unified_memory_model(wind.underlying());
    }

    auto local()       { return wind.local(); }
    auto local() const { return wind.local(); }

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

    auto win()       ->       auto& { return wind; }
    auto win() const -> const auto& { return wind; }
};


} // std_e
