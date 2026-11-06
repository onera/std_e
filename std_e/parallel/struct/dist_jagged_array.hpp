#pragma once


#include "std_e/parallel/mpi/one_sided/window.hpp"
#include "std_e/parallel/distribution.hpp"
#include "std_e/parallel/struct/dist_guard.hpp"


namespace std_e {

template<class T>
class dist_jagged_array {
  private:
    dist_array<int> displs;
    dist_array<int> stris;
    dist_array<T> vals;
  public:
    using value_type = T;

    dist_jagged_array(const interval_vector<int>& ds, MPI_Comm comm) { // TODO displs directly alloc as dist_array
      displs = dist_array<int>(ds.size(),comm);
      { dist_guard _(displs);
        displs.local() = ds;
      }
      stris = dist_array<int>(displs.size()-1,comm);
      { dist_guard _(stris);
        stris.local() = interval_lengths(ds);
      }
      vals = dist_array<T>(ds.back(),comm);
    }

    auto n_elt() const -> MPI_Aint { return displs.size(); }
    auto size() const -> MPI_Aint { return vals.size(); }

    auto displacements()       -> span_ref<      int> { return displs.local(); }
    auto displacements() const -> span_ref<const int> { return displs.local(); }
    auto strides      ()       -> span_ref<      int> { return stris .local(); }
    auto strides      () const -> span_ref<const int> { return stris .local(); }
    auto values       ()       -> span_ref<      T  > { return vals  .local(); }
    auto values       () const -> span_ref<const T  > { return vals  .local(); }

    // TODO jagged_ref

    auto n_rank() const {
      return vals.n_rank();
    }

    auto displacements_dist_array() -> auto& { return displs; }
    auto strides_dist_array      () -> auto& { return stris ; }
    auto values_dist_array       () -> auto& { return vals  ; }

    //auto strides_win()       ->       auto& { return stris.win(); }
    //auto strides_win() const -> const auto& { return stris.win(); }
    //auto values_win ()       ->       auto& { return vals .win(); }
    //auto values_win () const -> const auto& { return vals .win(); }

    auto comm() const -> MPI_Comm { return vals.comm(); }
};


} // std_e
