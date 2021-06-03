#include "std_e/parallel/all_to_all.hpp"

#include "doctest/extensions/doctest_mpi.h"
#include "std_e/parallel/mpi.hpp"

using namespace std;

MPI_TEST_CASE("all_gather",3) {
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

MPI_TEST_CASE("all_gather_alt",3) {
  int value;
  if (test_rank==0) {
    value = 10;
  } else if (test_rank==1) {
    value = 5;
  } else {
    STD_E_ASSERT(std_e::rank(test_comm)==2);
    value = 7;
  }

  vector<int> data_received = std_e::all_gather(value,test_comm);

  CHECK( data_received == vector{10,5,7} );
}

MPI_TEST_CASE("all_gather_vect",3) {
  vector<int> value;
  if (test_rank==0) {
    value = {1,2,3};
  } else if (test_rank==1) {
    value = {4,5,6,7};
  } else {
    STD_E_ASSERT(std_e::rank(test_comm)==2);
    value = {8,9,10,11,12};
  }

  std::vector<int> rdata = std_e::all_gather(value,test_comm);
  
  CHECK(rdata == vector{1,2,3,4,5,6,7,8,9,10,11,12} );
}
 
