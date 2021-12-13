#include "std_e/unit_test/doctest_mpi.hpp"

#include "std_e/parallel/algorithm/partition_sort.hpp"
#include "std_e/interval/algorithm.hpp"
#include "std_e/unit_test/math.hpp"

using namespace std_e;

TEST_CASE("number_of_ticks_in_interval") {
  int n = 4;
  int k = 2;
  /*  0   1   2   3   
      |---|---|---|---|
           ^     ^
           |_____| the two ticks should end up here
  */

  LOG("000");
  CHECK( number_of_ticks_in_interval(n,k,0) == 0 );
  LOG("001");
  CHECK( number_of_ticks_in_interval(n,k,1) == 1 );
  LOG("002");
  CHECK( number_of_ticks_in_interval(n,k,2) == 1 );
  LOG("003");
  CHECK( number_of_ticks_in_interval(n,k,3) == 0 );
  LOG("004");
}

MPI_TEST_CASE("parallel partition_sort",4) {
  int sz_tot = 200;
  int rk = test_rank;
  int n_rk = test_nb_procs;

  int sz = sz_tot/n_rk;
  interval<double> in = interval_portion({-5.,10.},n_rk,rk); // distribute [-5.,10.) over the procs
  std::vector<double> y = function_result_vector(sinc,sz,in); // take `sz` sample points of the sinc function over domain `in`

  SUBCASE("partition_sort_once") {
    interval_vector<int> partition_indices = std_e::partition_sort_once(y,test_comm);
    CHECK( partition_indices.size() == n_rk+1 );

    // test that the array `y` is indeed partitioned ...
    // 0. ... for that, compute the min and max of each partition ...
    auto [min_by_partition,max_by_partition] = minmax_over_interval_sequence(y,partition_indices);
    // 1. ... and check that the max of one partition is less than the min of the next one
    for (int i=0; i<int(min_by_partition.size())-1; ++i) {
      CHECK( max_by_partition[i] < min_by_partition[i+1] );
    }

    //auto partition_indices_tot = all_reduce(partition_indices.as_base(),MPI_SUM,test_comm);
    //ELOG(interval_lengths(partition_indices));
    //ELOG(partition_indices);
    //ELOG(partition_indices_tot);
    //ELOG(interval_lengths(partition_indices_tot));
    //ELOG(make_span(y.data()+partition_indices[1],y.data()+partition_indices[2]));
  }

  SUBCASE("partition_sort_minimize_imbalance") {
    interval_vector<int> partition_indices = std_e::partition_sort_minimize_imbalance(y,sz_tot,test_comm,0.4);
    auto partition_indices_tot = all_reduce(partition_indices.as_base(),MPI_SUM,test_comm);
    ELOG(interval_lengths(partition_indices_tot));
  }

}
