#include "std_e/parallel/dist_graph.hpp"

namespace std_e {

auto
dist_graph_create(MPI_Comm comm_old, const std::vector<int>& out_ranks) -> MPI_Comm {
  MPI_Comm comm_dist_graph;
  int i_rank = rank(comm_old);
  int n_dest = out_ranks.size();
  std::vector<int> weights(n_dest,1);
  int err = MPI_Dist_graph_create(comm_old, 1, &i_rank, &n_dest, out_ranks.data(),
                                  weights.data(), MPI_INFO_NULL, 0/*no reorder*/, &comm_dist_graph);
  if (err!=0) throw mpi_exception(err,std::string("in function \"")+__func__+"\"");
  return comm_dist_graph;
}
auto
dist_graph_create_adj(MPI_Comm comm_old, const std::vector<int>& in_ranks, const std::vector<int>& out_ranks) -> MPI_Comm {
  STD_E_ASSERT(in_ranks.data()!=out_ranks.data()); // MPI implementations are not fond of pointer aliasing
  MPI_Comm comm_dist_graph;
  int in_degree = in_ranks.size();
  int out_degree = out_ranks.size();
  std::vector<int> in_weights(in_degree,1);
  std::vector<int> out_weights(out_degree,1);
  int err = MPI_Dist_graph_create_adjacent(
    comm_old,
    in_degree, in_ranks.data(), in_weights.data(),
    out_degree, out_ranks.data(), out_weights.data(),
    MPI_INFO_NULL, 0/*no reorder*/, &comm_dist_graph
  );
  if (err!=0) throw mpi_exception(err,std::string("in function \"")+__func__+"\"");
  return comm_dist_graph;
}
auto
dist_graph_create_adj(MPI_Comm comm_old, const std::vector<int>& in_out_ranks) -> MPI_Comm {
  std::vector<int> copy = in_out_ranks;  // MPI implementations are not fond of pointer aliasing
  return dist_graph_create_adj(comm_old,in_out_ranks,copy);
}

} // std_e
