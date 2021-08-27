#pragma once


#include <mpi.h>


namespace std_e {


using g_num = int64_t;


inline auto
rank(MPI_Comm comm) -> int {
  int i;
  MPI_Comm_rank(comm, &i);
  return i;
}
inline auto
n_rank(MPI_Comm comm) -> int {
  int n;
  MPI_Comm_size(comm, &n);
  return n;
}


/// Can be safely called before MPI_Init (useful for initializing globals because MPI_Init cannot be called before main())
inline auto
mpi_comm_world_rank() -> int {
  #if defined(OPEN_MPI)
    const char* rank_str = std::getenv("OMPI_COMM_WORLD_RANK");
  #elif defined(I_MPI_VERSION) || defined(MPI_VERSION)
    const char* rank_str = std::getenv("MPI_LOCALRANKID"); // env name used at least by Intel MPI
  #else
    #error "Unknown MPI implementation"
  #endif
  if (rank_str==nullptr) return 0; // not launched with mpirun/mpiexec...
  return std::stoi(rank_str);
}




// to_mpi_type {
template<class T> constexpr auto
to_mpi_type__impl() -> MPI_Datatype {
  constexpr int sz = sizeof(T);
       if constexpr (sz==1) return MPI_BYTE;
  else if constexpr (sz==2) return MPI_INT16_T;
  else if constexpr (sz==4) return MPI_INT32_T;
  else if constexpr (sz==8) return MPI_INT64_T;
  else throw mpi_exception(-1,"sizeof primitive type should by 1, 2, 4 or 8");
}

template<class T> constexpr MPI_Datatype to_mpi_type = to_mpi_type__impl<T>();
// to_mpi_type }


} // std_e
