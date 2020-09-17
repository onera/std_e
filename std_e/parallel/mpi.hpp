#pragma once


#include <mpi.h>
#include "std_e/utils/tuple.hpp"
#include "std_e/future/make_array.hpp"
#include "std_e/future/contract.hpp"


namespace std_e {


inline auto
mpi_init_if_needed() -> bool {
  int flag;
  MPI_Initialized(&flag);
  bool already_init = bool(flag);

  if (!already_init) {
    MPI_Init(NULL, NULL);
  }
  return already_init;
}


// to_mpi_type {
template<class T> constexpr auto
to_mpi_type__impl() -> MPI_Datatype {
  constexpr int sz = sizeof(T);
  static_assert(sz==1 || sz==2 || sz==4 || sz==8,"no corresponding MPI type");
       if constexpr (sz==1) return MPI_BYTE;
  else if constexpr (sz==2) return MPI_INT16_T;
  else if constexpr (sz==4) return MPI_INT32_T;
  else if constexpr (sz==8) return MPI_INT64_T;
}

template<class T> constexpr MPI_Datatype to_mpi_type = to_mpi_type__impl<T>();
// to_mpi_type }


inline auto
rank(MPI_Comm comm) -> int {
  int i;
  MPI_Comm_rank(comm, &i);
  return i;
}
inline auto
nb_ranks(MPI_Comm comm) -> int {
  int n;
  MPI_Comm_size(comm, &n);
  return n;
}


template<class T> auto
min_global(T local_min, MPI_Comm comm) -> T {
  T global_min = std::numeric_limits<T>::max();

  int ierr = MPI_Allreduce(&local_min, &global_min, 1, to_mpi_type<T>, MPI_MIN, comm);
  STD_E_ASSERT(ierr == 0);

  return global_min;
}
template<class T> auto
max_global(T local_max, MPI_Comm comm) -> T {
  T global_max = std::numeric_limits<T>::min();

  int ierr = MPI_Allreduce(&local_max, &global_max, 1, to_mpi_type<T>, MPI_MAX, comm);
  STD_E_ASSERT(ierr == 0);

  return global_max;
}
template<class T> auto
minmax_global(T local_min, T local_max, MPI_Comm comm) -> std::pair<T,T> {
  return {min_global(local_min,comm),max_global(local_max,comm)};
}


} // std_e
