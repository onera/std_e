#include "std_e/parallel/all_to_all.hpp"
#include "std_e/parallel/dist_graph.hpp"

#include "std_e/unit_test/mpi/doctest.hpp"


MPI_TEST_CASE("dist_graph_create",4) {
  std::vector<int> destination_ranks;
  if (test_rank==0) { destination_ranks = {1,3}; }
  if (test_rank==1) { destination_ranks = {0}  ; }
  if (test_rank==2) { destination_ranks = {3}  ; }
  if (test_rank==3) { destination_ranks = {0,2}; }
  MPI_Comm comm_graph = std_e::dist_graph_create(test_comm,destination_ranks);

  using data_t = std::vector<int>;

  std::vector<data_t> data_to_send;
  if (test_rank==0) {
    data_to_send.push_back( {1,2} );
    data_to_send.push_back( {3,4,5} );
  } else if (test_rank==1) {
    data_to_send.push_back( {9,10,11,12} );
  } else if (test_rank==2) {
    data_to_send.push_back( {6} );
  } else if (test_rank==3) {
    data_to_send.push_back( {14} );
    data_to_send.push_back( {15,16} );
  }

  std::vector<data_t> data_received = std_e::neighbor_all_to_all_v(data_to_send,comm_graph);

  // TODO reception order is unspecified
  //MPI_CHECK( 0, data_received == std::vector<data_t>{{9,10,11,12},{14}} );
  MPI_CHECK( 1, data_received == std::vector<data_t>{{1,2}}             );
  MPI_CHECK( 2, data_received == std::vector<data_t>{{15,16}}           );
  //MPI_CHECK( 3, data_received == std::vector<data_t>{{6},{3,4,5}}       );
}
