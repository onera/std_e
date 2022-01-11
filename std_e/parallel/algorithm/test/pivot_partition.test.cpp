//#include "std_e/unit_test/doctest_mpi.hpp"
//
//#include "std_e/parallel/algorithm/pivot_partition.hpp"
//#include "std_e/interval/algorithm.hpp"
//#include "std_e/unit_test/math.hpp"
//
//using namespace std_e;
//using std::vector;
//
//MPI_TEST_CASE("parallel pivot_partition - 2 procs",2) {
//  int rk = test_rank;
//
//  vector<int> x;
//  if (rk == 0) x = {13,11,10,14,0,7,9,6};
//  if (rk == 1) x = {12,3,4,8,5,1,2};
//
//  double max_imbalance = 0.;
//  interval_vector<int> partition_indices = std_e::pivot_partition_minimize_imbalance(x,test_comm,max_imbalance);
//
//  // check that `pivot_partition_once` did partition `x` according to the returned indices
//  CHECK( is_partitioned_at_indices(x,partition_indices) );
//
//  // Since max_imbalance==0., it means the global partition index is exactly the middle
//  //   1. there are 3 partition_indices...
//  CHECK( partition_indices.size() == 3 );
//  //     ... but the first is 0, the last is the local size, so only partition_indices[1] is of interest
//  //   2. the global partition index is the sum over the ranks of partition_indices[1]
//  CHECK( all_reduce(partition_indices[1],MPI_SUM,test_comm) == 8 );
//
//  // regression testing values
//  MPI_CHECK( 0, partition_indices == interval_vector{0,       3,        8} );
//  MPI_CHECK( 0,                 x ==          vector{0,6,7,9,10,13,14,11} );
//  MPI_CHECK( 1, partition_indices == interval_vector{0,        5,  7} );
//  MPI_CHECK( 1,                 x ==          vector{2,3,4,1,5,8,12} );
//}
//
//// WILL NOT WORK: no progress by repeatedly selecting the same pivot
////MPI_TEST_CASE("parallel pivot_partition - 3 procs",3) {
////  int rk = test_rank;
////  int n_rk = test_nb_procs;
////
////  vector<int> x;
////  //if (rk == 0) x = {0,1,2,3,4};
////  //if (rk == 1) x = {5,6,7,8,9};
////  //if (rk == 2) x = {10,11,12,13,14};
////  if (rk == 0) x = {13,11,10,14,0};
////  if (rk == 1) x = {12,3,4,8};
////  if (rk == 2) x = {7,9,6,5,1,2};
////
////  double max_imbalance = 0.;
////  interval_vector<int> partition_indices = std_e::pivot_partition_minimize_imbalance(x,test_comm,max_imbalance);
////
////  ELOG(partition_indices);
////  ELOG(x);
////
////  //int sz_tot = 15;
////  //if (rk == 0) x = {0,1,2,3,4};
////  //if (rk == 1) x = {5,6,7,8,9};
////  //if (rk == 2) x = {10,11,12,13,14};
////
////  //int sz_tot = 18;
////  //if (rk == 0) x = {13,11,10,14,17,0};
////  //if (rk == 1) x = {12,16,3,4,8};
////  //if (rk == 2) x = {7,9,6,15,5,1,2};
////}
//
//
//////MPI_TEST_CASE("parallel pivot_partition - cardinal sine function",16) {
//////  int sz_tot = 256'000'000;
////MPI_TEST_CASE("parallel pivot_partition - cardinal sine function",4) {
////  int sz_tot = 200;
////  int rk = test_rank;
////  int n_rk = test_nb_procs;
////
////  int sz = sz_tot/n_rk;
////  interval<double> in = interval_portion({-5.,10.},n_rk,rk); // distribute [-5.,10.) over the procs
////  vector<double> y = function_result_vector(sinc,sz,in); // take `sz` sample points of the sinc function over domain `in`
////
////  SUBCASE("pivot_partition_minimize_imbalance") {
////    double max_imbalance = 0.1;
////    interval_vector<int> partition_indices = std_e::pivot_partition_minimize_imbalance(y,test_comm,max_imbalance);
////
////    CHECK( is_partitioned_at_indices(y,partition_indices) );
////
////    auto partition_indices_tot = all_reduce(partition_indices.as_base(),MPI_SUM,test_comm);
////    auto lens = interval_lengths(partition_indices_tot);
////    int optimal_len = sz;
////    for (int i=0; i<int(lens.size())-1; ++i) {
////      CHECK( std::abs(lens[i]-optimal_len)/double(optimal_len) < max_imbalance );
////    }
////
////    //if (rk==0) {
////    //  ELOG(partition_indices_tot);
////    //  ELOG(lens);
////    //}
////  }
////}
