#include "std_e/parallel/all_to_all.hpp"

#include "doctest/extensions/doctest_mpi.h"
#include "std_e/parallel/mpi.hpp"

using namespace std;

MPI_TEST_CASE("all_gather",3) {
  using data_t = vector<int>;
  std::vector<data_t> data_to_send(3);
  int value;
  if (test_rank==0) {
    value = 10;
  } else if (test_rank==1) {
    value = 5;
  } else {
    STD_E_ASSERT(std_e::rank(test_comm)==2);
    value = 7;
  }

  vector<int> data_received(3);

  std_e::all_gather(value,data_received.data(),test_comm);

  CHECK( data_received == vector{10,5,7} );
}
