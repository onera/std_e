#pragma once


#include "std_e/parallel/mpi/one_sided/window.hpp"
#include "std_e/parallel/struct/distribution.hpp"


namespace std_e {

template<class T>
class dist_array {
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
    dist_array(const std::vector<I>& distri, MPI_Comm comm) // TODO Distribution
      : dist_array(distri[rank(comm)+1]-distri[rank(comm)],comm) // TODO
    {}

    template<class I>
    dist_array(const distribution_vector<I>& distri, MPI_Comm comm)
      //: dist_array(distri.length(rank(comm)),comm)
      : dist_array(distri[rank(comm)+1]-distri[rank(comm)],comm) // TODO
    {}

    dist_array(const dist_array&) = delete;
    dist_array& operator=(const dist_array&) = delete;
    dist_array(dist_array&&) = default;
    dist_array& operator=(dist_array&&) = default;

    auto size() const -> MPI_Aint { return w.size(); }
    auto local()       -> span_ref<      T> { return w.local(); }
    auto local() const -> span_ref<const T> { return w.local(); }

    auto n_rank() const {
      return w.n_rank();
    }

    auto win()       ->       auto& { return w; }
    auto win() const -> const auto& { return w; }

    auto comm() const -> MPI_Comm { return w.communicator(); }
};


} // std_e
