#if __cplusplus > 201703L
#include "std_e/parallel/all_to_all.hpp"

#include "doctest/extensions/doctest_mpi.h"

using namespace std;

MPI_TEST_CASE("all_to_all_v",3) {
  using data_t = vector<int>;
  std::vector<data_t> data_to_send(3);
  if (test_rank==0) {
    data_to_send[0] = {1,2};
    data_to_send[1] = {3,4,5};
    data_to_send[2] = {6,7};
  } else if (test_rank==1) {
    data_to_send[0] = {8};
    data_to_send[1] = {9,10,11,12};
    data_to_send[2] = {13};
  } else {
    STD_E_ASSERT(std_e::rank(test_comm)==2);
    data_to_send[0] = {};
    data_to_send[1] = {14};
    data_to_send[2] = {15,16};
  }

  vector<data_t> data_received = std_e::all_to_all_v(data_to_send,test_comm);

  MPI_CHECK(0, data_received[0] == vector{1,2} );
  MPI_CHECK(0, data_received[1] == vector{8} );
  MPI_CHECK(0, data_received[2] == vector<int>{} );

  MPI_CHECK(1, data_received[0] == vector{3,4,5} );
  MPI_CHECK(1, data_received[1] == vector{9,10,11,12} );
  MPI_CHECK(1, data_received[2] == vector{14} );

  MPI_CHECK(2, data_received[0] == vector{6,7} );
  MPI_CHECK(2, data_received[1] == vector{13} );
  MPI_CHECK(2, data_received[2] == vector{15,16} );
}
#endif // C++>17
