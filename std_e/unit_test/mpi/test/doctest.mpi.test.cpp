//#include "std_e/unit_test/mpi/doctest.hpp"
//#include "std_e/parallel/mpi.hpp"
//
//int f_for_test(MPI_Comm comm) {
//  int rank = std_e::rank(comm);
//  if (rank == 0) {
//    return 10;
//  }
//  else if (rank == 1) {
//    return 11;
//  }
//  return 0;
//}
//
//
//MPI_TEST_CASE("test 0",2) { // Parallel test on 2 processes
//  // if MPI_SIZE < 2, report test can't be run
//  // if MPI_SIZE >= 2, create a sub-communicator mpi_test_comm of size 2
//  int x = f_for_test(test_comm);
//  
//  MPI_CHECK( 0,  x==10 ); // CHECK for rank 0, that x==10
//  MPI_CHECK( 1,  x==11 ); // CHECK for rank 1, that x==11
//  //MPI_CHECK( 2,  x==0 ); // will trigger a static assert because non-existing rank
//}
//
//TEST_CASE("test 1 with 1") {
//  CHECK( 1==2 );
//}
//
//MPI_TEST_CASE("test 1",3) { // Parallel test on 3 processes
//  int x = f_for_test(test_comm);
//  
//  MPI_CHECK( 0,  x==10 ); // CHECK for rank 0, that x==10
//  MPI_CHECK( 1,  x==11 ); // CHECK for rank 1, that x==11
//  MPI_CHECK( 2,  x==-1 ); // CHECK for rank 2, that x==-1 (which is not the case -> will trigger a failure report)
//}
//
//TEST_CASE("test 2 with 1") {
//  CHECK( 1==3 );
//}
