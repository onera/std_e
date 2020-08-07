#pragma once

#include "std_e/parallel/mpi.hpp"

inline auto
init_mpi() -> MPI_Comm {
  std_e::mpi_init_if_needed();
  MPI_Comm mpi_comm = MPI_COMM_WORLD;
  return mpi_comm;
}

/*
 *  \brief magic_static to get a global variable containing comm_world
 */
static auto // TODO static not needed?
get_comm_world() -> MPI_Comm& {
  // See C++ Magic Statics (N2660)
  static MPI_Comm mpi_comm = init_mpi();
  return mpi_comm;
}
