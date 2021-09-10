#pragma once

#include <vector>
#include "std_e/parallel/mpi.hpp"

namespace std_e {

auto dist_graph_create(MPI_Comm comm_old, const std::vector<int>& out_ranks) -> MPI_Comm;

auto dist_graph_create_adj(MPI_Comm comm_old, const std::vector<int>& in_ranks, const std::vector<int>& out_ranks) -> MPI_Comm;
auto dist_graph_create_adj(MPI_Comm comm_old, const std::vector<int>& in_out_ranks) -> MPI_Comm;

} // std_e
