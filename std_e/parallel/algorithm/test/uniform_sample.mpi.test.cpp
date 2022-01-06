#include "std_e/unit_test/doctest_mpi.hpp"

#include "std_e/parallel/algorithm/uniform_sample.hpp"
#include "std_e/log.hpp" // TODO

using namespace std;
using namespace std_e;

MPI_TEST_CASE("uniform_sample_local",4) {
  vector<int> x;
  if (test_rank==0) x = {0,1,2,3,4};
  if (test_rank==1) x = {5,6,7};
  if (test_rank==2) x = {};
  if (test_rank==3) x = {8,9};

  vector<int> sample_local_2 = uniform_sample_local(x,2,test_comm);
  MPI_CHECK( 0 , sample_local_2 == vector{4} );
  MPI_CHECK( 1 , sample_local_2 == vector{7} );
  MPI_CHECK( 2 , sample_local_2 == vector<int>{} );
  MPI_CHECK( 3 , sample_local_2 == vector<int>{} );

  vector<int> sample_local_5 = uniform_sample_local(x,5,test_comm);
  MPI_CHECK( 0 , sample_local_5 == vector{2,4} );
  MPI_CHECK( 1 , sample_local_5 == vector{6} );
  MPI_CHECK( 2 , sample_local_5 == vector<int>{} );
  MPI_CHECK( 3 , sample_local_5 == vector{8,9} );

  //vector<int> sample_local_9 = uniform_sample_local(x,9,test_comm);
  ////MPI_CHECK( 0 , sample_local_10 == x );
  //MPI_CHECK( 1 , sample_local_9 == x );
  //MPI_CHECK( 2 , sample_local_9 == x );
  //MPI_CHECK( 3 , sample_local_9 == x );
}
