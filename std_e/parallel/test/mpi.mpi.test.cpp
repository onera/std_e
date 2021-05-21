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


MPI_TEST_CASE("all_gather_vect",3) {
  vector<int> value;
  if (test_rank==0) {
    value = {1,2,3};
  } else if (test_rank==1) {
    value = {4,5,6,8};
  } else {
    STD_E_ASSERT(std_e::rank(test_comm)==2);
    value = {7,8,9,10,11};
  }

  int sz = value.size();
  vector<int> data_sizes(3);

  std_e::all_gather(sz,data_sizes.data(),test_comm);
  int total_sz = std::accumulate(data_sizes.begin(),data_sizes.end(),0); 

  vector<int> rdata(total_sz);
  vector<int> displs(3);
  
  for (auto i = 0; i<displs.size(); ++i)
  {
    displs[i] = accumulate(data_sizes.begin(),data_sizes.begin()+i,0);
  }

  for(auto ii:displs) cout << "displs : " << ii << endl;

  CHECK( total_sz == 10 );

  std_e::all_gather(value, rdata.data(), data_sizes, displs, test_comm);
  
  if (test_rank == 0){
    for(auto ii:data_sizes) cout << "data_sizes : " << ii << endl;
    for(auto ii:rdata) cout << "recdata : "<< ii << endl;
  }
//int err = MPI_Allgatherv(value.data(), value.size(), to_mpi_type<T>,
  //                         rbuf.data(), recvcounts.data(), displs.data(), to_mpi_type<T>, comm);
  //if (err!=0) throw mpi_exception(err,std::string("in function \"")+__func__+"\"");
}
 
