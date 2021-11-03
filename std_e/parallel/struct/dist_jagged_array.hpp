#pragma once


#include "std_e/parallel/mpi/one_sided/window.hpp"
#include "std_e/parallel/struct/distribution.hpp"
#include "std_e/parallel/struct/dist_guard.hpp"


namespace std_e {

template<class T>
class dist_jagged_array {
  private:
    interval_vector<int> idces;
    dist_array<int> szs;
    dist_array<T> vals;
  public:
    using value_type = T;

    dist_jagged_array(interval_vector<int>&& is, MPI_Comm comm)
      : idces(std::move(is))
    {
      szs = dist_array<int>(idces.size()-1,comm);
      { dist_guard _(szs);
        szs.local() = interval_lengths(idces);
      }
      vals = dist_array<T>(idces.back(),comm);
    }

    auto n_elt() const -> MPI_Aint { return idces.size(); }
    auto size() const -> MPI_Aint { return vals.size(); }

    auto indices()       -> span_ref<      int> { return idces       ; }
    auto indices() const -> span_ref<const int> { return idces       ; }
    auto sizes  ()       -> span_ref<      int> { return szs .local(); }
    auto sizes  () const -> span_ref<const int> { return szs .local(); }
    auto values ()       -> span_ref<      T  > { return vals.local(); }
    auto values () const -> span_ref<const T  > { return vals.local(); }

    // TODO jagged_ref

    auto n_rank() const {
      return vals.n_rank();
    }

    auto sizes_dist_array() -> auto& { return szs; }
    auto value_dist_array() -> auto& { return vals; }

    auto sizes_win()       ->       auto& { return szs .win(); }
    auto sizes_win() const -> const auto& { return szs .win(); }
    auto value_win()       ->       auto& { return vals.win(); }
    auto value_win() const -> const auto& { return vals.win(); }

    auto comm() const -> MPI_Comm { return vals.comm(); }
};


} // std_e
