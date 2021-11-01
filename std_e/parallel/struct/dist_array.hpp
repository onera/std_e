#pragma once


#include "std_e/parallel/mpi/one_sided/window.hpp"
#include "std_e/parallel/struct/distribution.hpp"


namespace std_e {

template<class T>
class dist_array {
    // invariants:
    //   - handle refers to an object which is valid over comm
    //   - distri is of size n_rank+1
    //   - local_block.size() == length(distri,i_rank)
  private:
    window<T> w;
  public:
    using value_type = T;

    dist_array() = default;

    dist_array(MPI_Aint sz, MPI_Comm comm)
      : w(sz,comm)
    {
      check_unified_memory_model(w.underlying());
    }
    template<class I>
    dist_array(const distribution_vector<I>& distri, MPI_Comm comm)
      : dist_array(distri.length(rank(comm)),comm)
    {}

    dist_array(const dist_array&) = delete;
    dist_array& operator=(const dist_array&) = delete;
    dist_array(dist_array&&) = default;
    dist_array& operator=(dist_array&&) = default;

    auto local()       { return w.local(); }
    auto local() const { return w.local(); }

    auto size() const {
      return w.size();
    }

    auto n_rank() const {
      return w.n_rank();
    }

    auto win()       ->       auto& { return w; }
    auto win() const -> const auto& { return w; }
};


} // std_e
