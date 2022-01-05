#include "std_e/unit_test/doctest_mpi.hpp"

#include "std_e/parallel/algorithm/pivot_partition_eq.hpp"
#include "std_e/interval/algorithm.hpp"
#include "std_e/unit_test/math.hpp"

using namespace std_e;

MPI_TEST_CASE("median_of_3_sample",3) {
  int rk = test_rank;
  std::vector<int> x;
  if (rk == 0) x = { 0, 1, 2, 3, 4, 5};
  if (rk == 1) x = { 6, 7, 8, 9,10,11};
  if (rk == 2) x = {12,13,14,15,16,17};

  std::vector<int> sample = std_e::median_of_3_sample(x,test_comm);

  CHECK(sample == std::vector{5,13});
}

MPI_TEST_CASE("parallel pivot_partition_once",3) {
  int rk = test_rank;
  std::vector<int> x;
  //if (rk == 0) x = {0,1,2,3,4};
  //if (rk == 1) x = {5,6,7,8,9};
  //if (rk == 2) x = {10,11,12,13,14};
  if (rk == 0) x = {13,11,10,14,0};
  if (rk == 1) x = {12,3,4,8};
  if (rk == 2) x = {7,9,6,5,1,2};

  interval_vector<int> partition_indices = std_e::pivot_partition_once(x,test_comm);

  ELOG(partition_indices);
  ELOG(x);
}
  //int sz_tot = 15;
  //if (rk == 0) x = {0,1,2,3,4};
  //if (rk == 1) x = {5,6,7,8,9};
  //if (rk == 2) x = {10,11,12,13,14};

  //int sz_tot = 18;
  //if (rk == 0) x = {13,11,10,14,17,0};
  //if (rk == 1) x = {12,16,3,4,8};
  //if (rk == 2) x = {7,9,6,15,5,1,2};

//MPI_TEST_CASE("parallel pivot_partition_eq",16) {
//  int sz_tot = 256'000'000;
MPI_TEST_CASE("parallel pivot_partition_once - cardinal sine function",4) {
  int sz_tot = 200;
  int rk = test_rank;
  int n_rk = test_nb_procs;

  int sz = sz_tot/n_rk;
  interval<double> x = interval_portion({-5.,10.},n_rk,rk); // distribute [-5.,10.) over the procs
  std::vector<double> y = function_result_vector(sinc,sz,x); // take `sz` sample points of the sinc function over domain `x`

  interval_vector<int> partition_indices = std_e::pivot_partition_once(y,test_comm);
  CHECK( partition_indices.size() == n_rk+1 );

  // test that the array `y` is indeed partitioned
  CHECK( is_partitioned_at_indices(y,partition_indices) );

  // Note: the following numbers are hard to interpret for a "tutorial" unit test
  //       but they are here for non-regression (we did not find a way to express the unit test better!)
  MPI_CHECK( 0, partition_indices == interval_vector{0,20,28,50,50} );
  MPI_CHECK( 1, partition_indices == interval_vector{0, 0, 0,32,50} );
  MPI_CHECK( 2, partition_indices == interval_vector{0,26,44,50,50} );
  MPI_CHECK( 3, partition_indices == interval_vector{0, 0,25,50,50} );
}
