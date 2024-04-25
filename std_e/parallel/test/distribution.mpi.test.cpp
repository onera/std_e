#include "std_e/unit_test/doctest_mpi.hpp"

#include "std_e/parallel/partial_distribution.hpp"

using namespace std_e;

MPI_TEST_CASE("partial_distribution",4) {
  int sz;
  if (test_rank==0) sz = 3;
  if (test_rank==1) sz = 0;
  if (test_rank==2) sz = 5;
  if (test_rank==3) sz = 1;

  partial_distri<int> d = partial_distribution(sz,test_comm);

  MPI_CHECK( 0 , d == partial_distri<int>{0, 3, 9} );
  MPI_CHECK( 1 , d == partial_distri<int>{3, 3, 9} );
  MPI_CHECK( 2 , d == partial_distri<int>{3, 8, 9} );
  MPI_CHECK( 3 , d == partial_distri<int>{8, 9, 9} );

  MPI_CHECK( 0 , to_string(d) == "(0,3,9)" );
}
