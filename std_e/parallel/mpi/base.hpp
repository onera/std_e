#pragma once


#include <mpi.h>
#include "std_e/parallel/mpi/mpi_exception.hpp"


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


// Can be safely called before MPI_Init()
// useful for initializing globals because MPI_Init() cannot be called before main()
inline auto
mpi_comm_world_rank() -> int {
  #if defined(OPEN_MPI)
    const char* rank_str = std::getenv("OMPI_COMM_WORLD_RANK");
  #elif defined(I_MPI_VERSION) || defined(MPI_VERSION)
    // REF https://community.intel.com/t5/Intel-oneAPI-HPC-Toolkit/Environment-variables-defined-by-intel-mpirun/td-p/1096703
    const char* rank_str = std::getenv("PMI_RANK"); // env name used at least by Intel MPI
  #else
    #error "Unknown MPI implementation"
  #endif
  if (rank_str==nullptr) return 0; // not launched with mpirun/mpiexec, so assume only one process
  return std::stoi(rank_str);
}

inline auto
mpi_comm_world_size() -> int {
  #if defined(OPEN_MPI)
    const char* size_str = std::getenv("OMPI_COMM_WORLD_SIZE");
  #elif defined(I_MPI_VERSION) || defined(MPI_VERSION)
    // REF https://community.intel.com/t5/Intel-oneAPI-HPC-Toolkit/Environment-variables-defined-by-intel-mpirun/td-p/1096703
    const char* size_str = std::getenv("PMI_SIZE"); // env name used at least by Intel MPI
  #else
    #error "Unknown MPI implementation"
  #endif
  if (size_str==nullptr) return 1; // not launched with mpirun/mpiexec, so assume only one process
  return std::stoi(size_str);
}



// to_mpi_type {
constexpr auto
to_mpi_type_of_size(int sz) -> MPI_Datatype {
       if (sz==1) return MPI_BYTE;
  else if (sz==2) return MPI_INT16_T;
  else if (sz==4) return MPI_INT32_T;
  else if (sz==8) return MPI_INT64_T;
  else throw mpi_exception(-1,"sizeof primitive type should by 1, 2, 4 or 8");
}

template<class T> constexpr MPI_Datatype to_mpi_type = to_mpi_type_of_size(sizeof(T));
// to_mpi_type }


} // std_e
