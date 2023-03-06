#include "std_e/unit_test/doctest_mpi.hpp"
#include "std_e/parallel/string.hpp"


MPI_TEST_CASE("cat_on_rank_0", 2) {
  auto cat_str = std_e::cat_on_rank_0("msg from rank "+std::to_string(test_rank)+"\n", test_comm);
  MPI_CHECK( 0 , cat_str == "msg from rank 0\nmsg from rank 1\n" ); // concatenation is ordered by rank
  MPI_CHECK( 1 , cat_str == "" );
}
