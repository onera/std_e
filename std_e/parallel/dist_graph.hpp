#pragma once

#include <vector>
#include "std_e/parallel/mpi.hpp"
#include "std_e/parallel/mpi_exception.hpp"

namespace std_e {

auto
dist_graph_create(MPI_Comm comm_old, const std::vector<int>& destination_ranks) -> MPI_Comm {
  MPI_Comm comm_dist_graph;
  int i_rank = rank(comm_old);
  int n_dest = destination_ranks.size();
  std::vector<int> weights(n_dest,1);
  int err = MPI_Dist_graph_create(comm_old, 1, &i_rank, &n_dest, destination_ranks.data(),
                                  weights.data(), MPI_INFO_NULL, 0/*no reorder*/, &comm_dist_graph);
  if (err!=0) throw mpi_exception(err,std::string("in function \"")+__func__+"\"");
  return comm_dist_graph;
}

} // std_e
