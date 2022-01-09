#include "std_e/unit_test/doctest_mpi.hpp"

#include "std_e/parallel/algorithm/pivot_partition.hpp"
#include "std_e/interval/algorithm.hpp"
#include "std_e/unit_test/math.hpp"

using namespace std_e;

////MPI_TEST_CASE("parallel pivot_partition - cardinal sine function",16) {
////  int sz_tot = 256'000'000;
//MPI_TEST_CASE("parallel pivot_partition - cardinal sine function",4) {
//  int sz_tot = 200;
//  int rk = test_rank;
//  int n_rk = test_nb_procs;
//
//  int sz = sz_tot/n_rk;
//  interval<double> in = interval_portion({-5.,10.},n_rk,rk); // distribute [-5.,10.) over the procs
//  std::vector<double> y = function_result_vector(sinc,sz,in); // take `sz` sample points of the sinc function over domain `in`
//
//  SUBCASE("pivot_partition_minimize_imbalance") {
//    double max_imbalance = 0.1;
//    interval_vector<int> partition_indices = std_e::pivot_partition_minimize_imbalance(y,sz_tot,test_comm,max_imbalance);
//
//    CHECK( is_partitioned_at_indices(y,partition_indices) );
//
//    auto partition_indices_tot = all_reduce(partition_indices.as_base(),MPI_SUM,test_comm);
//    auto lens = interval_lengths(partition_indices_tot);
//    int optimal_len = sz;
//    for (int i=0; i<int(lens.size())-1; ++i) {
//      CHECK( std::abs(lens[i]-optimal_len)/double(optimal_len) < max_imbalance );
//    }
//
//    //if (rk==0) {
//    //  ELOG(partition_indices_tot);
//    //  ELOG(lens);
//    //}
//  }
//}

//MPI_TEST_CASE("parallel pivot_partition",3) {
//  int sz_tot = 15;
//  int rk = test_rank;
//  int n_rk = test_nb_procs;
//
//  std::vector<int> x;
//  //if (rk == 0) x = {0,1,2,3,4};
//  //if (rk == 1) x = {5,6,7,8,9};
//  //if (rk == 2) x = {10,11,12,13,14};
//  if (rk == 0) x = {13,11,10,14,0};
//  if (rk == 1) x = {12,3,4,8};
//  if (rk == 2) x = {7,9,6,5,1,2};
//
//  double max_imbalance = 0.2;
//  interval_vector<int> partition_indices = std_e::pivot_partition_minimize_imbalance(x,sz_tot,test_comm,max_imbalance);
//  //interval_vector<int> partition_indices = std_e::pivot_partition_once(x,test_comm);
//
//  //ELOG(partition_indices);
//  //ELOG(x);
//
//  //int sz_tot = 15;
//  //if (rk == 0) x = {0,1,2,3,4};
//  //if (rk == 1) x = {5,6,7,8,9};
//  //if (rk == 2) x = {10,11,12,13,14};
//
//  //int sz_tot = 18;
//  //if (rk == 0) x = {13,11,10,14,17,0};
//  //if (rk == 1) x = {12,16,3,4,8};
//  //if (rk == 2) x = {7,9,6,15,5,1,2};
//}

//MPI_TEST_CASE("parallel pivot_partition",2) {
//  int sz_tot = 15;
//  int rk = test_rank;
//
//  std::vector<int> x;
//  if (rk == 0) x = {13,11,10,14,0,7,9,6};
//  if (rk == 1) x = {12,3,4,8,5,1,2};
//
//  double max_imbalance = 0.2;
//  interval_vector<int> partition_indices = std_e::pivot_partition_minimize_imbalance(x,sz_tot,test_comm,max_imbalance);
//
//  //ELOG(partition_indices);
//  //ELOG(x);
//}
