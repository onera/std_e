#include "std_e/unit_test/mpi/doctest.hpp"
#include "std_e/parallel/mpi.hpp"

int f_for_test(MPI_Comm comm) {
  int rank = std_e::rank(comm);
  if (rank == 0) {
    return 10;
  }
  else if (rank == 1) {
    return 11;
  }
  return 0;
}


MPI_TEST_CASE("test 0",2, // Parallel test on 2 processes
  // if MPI_SIZE < 2, report test can't be run
  // if MPI_SIZE >= 2, create a sub-communicator mpi_test_comm of size 2
  int x = f_for_test(test_comm);
  
  SUBCASE("by rank") {
    MPI_CHECK( 0,  x==10 ); // CHECK for rank 0, that x==10
    MPI_CHECK( 1,  x==11 ); // CHECK for rank 1, that x==11
  }

  //SUBCASE("check all ranks") {
  //  if (test_rank==0) {
  //    ++x;
  //  }
  //  MPI_CHECK_ALL_RANKS( x==11 ); // CHECK that x==11 for all ranks
  //}
)


MPI_TEST_CASE("test 1",3,  // Parallel test on 3 processes
  int x = f_for_test(test_comm);
  
  SUBCASE("by rank") {
    MPI_CHECK( 0,  x==10 ); // CHECK for rank 0, that x==10
    MPI_CHECK( 1,  x==11 ); // CHECK for rank 1, that x==11
    MPI_CHECK( 2,  x==-1  ); // CHECK for rank 2, that x==-1 (which is not the case)
  }

  //SUBCASE("check all ranks") {
  //  MPI_CHECK_ALL_RANKS( x==11 ); // CHECK that x==11 for all ranks (which is not the case on ranks 0 and 2)
  //}
)


/*
MPI_TEST_CASE("name",nb_procs) {
  if (MPI_get_size < nb_procs) {
    report_unable_to_execute();
  } else {
    MPI_Comm MPI_COMM_TEST = MPI_create_sub_comm(MPI_COMM_WORLD,nb_procs);
    TEST_CASE("name") {
      # what is inside the MPI_TEST_CASE{}
    }
  }
}

MPI_CHECK( rank, condition ) { // WARNING: must be executed by every rank (else: deadlock)
  // check
  bool test_res = true;
  if (rank == MPI_get_rank) {
    test_res = execute(condition)
  }
  // comm to 0
  std::vector<bool> all_test_res = mpi_gather(0, test_res);
  // report if 0
  if (MPI_get_rank==0) {
    bool tes_on_rank = bool test_res[rank]
    if (!tes_on_rank) {
      report_error(rank,condition);
    }
  }
}

MPI_CHECK_ALL_RANKS: same as MPI_CHECK, but test for every rank
*/
  

/*
Reporting

[doctest] doctest version is "x.x.x"
[doctest] run with "--help" for options
===============================================================================
/path/to/mpi/tests/my_tests.test.cpp:11:
TEST CASE:  test 1
  by rank

/path/to/mpi/tests/my_tests.test.cpp:21: ERROR: MPI_CHECK( 2, x==-1 ) is NOT correct!
  values: CHECK( 0 == -1 )

===============================================================================
/path/to/mpi/tests/my_tests.test.cpp:11:
TEST CASE:  test 1
  check all ranks

/path/to/mpi/tests/my_tests.test.cpp:21: ERROR: MPI_CHECK_ALL_RANKS( x==11 ) is NOT correct!
  values: CHECK( 10 == 11 ) on rank 0
  values: CHECK( 0 == 11 ) on rank 2

===============================================================================
[doctest] test cases:    158 |    157 passed |      1 failed |      0 skipped
[doctest] assertions:    835 |    834 passed |      1 failed |
[doctest] Status: FAILURE!
*/
