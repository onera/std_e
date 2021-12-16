#include "std_e/unit_test/doctest_mpi.hpp"

#include "std_e/parallel/algorithm/partition_sort.hpp"
#include "std_e/interval/algorithm.hpp"
#include "std_e/unit_test/math.hpp"

using namespace std_e;

TEST_CASE("number_of_ticks_in_interval") {
  SUBCASE("n>k") {
    int n = 4;
    int k = 2;
    /*  0   1   2   3
        |---|---|---|---|
             ^     ^
             |_____| the two ticks should end up here
    */
    CHECK( number_of_ticks_in_interval(n,k,0) == 0 );
    CHECK( number_of_ticks_in_interval(n,k,1) == 1 );
    CHECK( number_of_ticks_in_interval(n,k,2) == 1 );
    CHECK( number_of_ticks_in_interval(n,k,3) == 0 );
  }
  SUBCASE("n-1==k") {
    int n = 4;
    int k = 3;
    /*  0   1   2   3
        |---|---|---|---|
            ^   ^   ^
            |___|___|
    */
    CHECK( number_of_ticks_in_interval(n,k,0) == 0 );
    CHECK( number_of_ticks_in_interval(n,k,1) == 1 );
    CHECK( number_of_ticks_in_interval(n,k,2) == 1 );
    CHECK( number_of_ticks_in_interval(n,k,3) == 1 );
  }
  SUBCASE("n<k") {
    int n = 4;
    int k = 7;
    CHECK( number_of_ticks_in_interval(n,k,0) == 1 );
    CHECK( number_of_ticks_in_interval(n,k,1) == 2 );
    CHECK( number_of_ticks_in_interval(n,k,2) == 2 );
    CHECK( number_of_ticks_in_interval(n,k,3) == 2 );
  }
}

TEST_CASE("ticks_in_interval") {
  // position 3 samples in [0,22,54,60,85)
  CHECK( ticks_in_interval( 0,22,85,3) == std::vector<int>{} );
  CHECK( ticks_in_interval(22,54,85,3) == std::vector{22,43} );
  CHECK( ticks_in_interval(54,60,85,3) == std::vector<int>{} );
  CHECK( ticks_in_interval(60,85,85,3) == std::vector{64} );
}

//MPI_TEST_CASE("parallel partition_sort",16) {
//  int sz_tot = 256'000'000;
MPI_TEST_CASE("parallel partition_sort",4) {
  int sz_tot = 200;
  int rk = test_rank;
  ELOG(rk);
  int n_rk = test_nb_procs;

  int sz = sz_tot/n_rk;
  interval<double> in = interval_portion({-5.,10.},n_rk,rk); // distribute [-5.,10.) over the procs
  std::vector<double> y = function_result_vector(sinc,sz,in); // take `sz` sample points of the sinc function over domain `in`

  //SUBCASE("partition_sort_once") {
  //  interval_vector<int> partition_indices = std_e::partition_sort_once(y,test_comm);
  //  CHECK( partition_indices.size() == n_rk+1 );

  //  // test that the array `y` is indeed partitioned ...
  //  // 0. ... for that, compute the min and max of each partition ...
  //  auto [min_by_partition,max_by_partition] = minmax_over_interval_sequence(y,partition_indices);
  //  // 1. ... and check that the max of one partition is less than the min of the next one
  //  for (int i=0; i<int(min_by_partition.size())-1; ++i) {
  //    CHECK( max_by_partition[i] < min_by_partition[i+1] );
  //  }

  //  // Note: the following numbers are hard to interpret for a "tutorial" unit test
  //  //       but they are here for non-regression (we did not find a way to express the unit test better!)
  //  MPI_CHECK( 0, partition_indices == interval_vector{0,20,28,50,50} );
  //  MPI_CHECK( 1, partition_indices == interval_vector{0, 0, 0,32,50} );
  //  MPI_CHECK( 2, partition_indices == interval_vector{0,26,44,50,50} );
  //  MPI_CHECK( 3, partition_indices == interval_vector{0, 0,25,50,50} );
  //  auto partition_indices_tot = all_reduce(partition_indices.as_base(),MPI_SUM,test_comm);
  //  if (rk==0) {
  //    //ELOG(interval_lengths(partition_indices));
  //    ELOG(partition_indices_tot);
  //    ELOG(interval_lengths(partition_indices_tot));
  //    //ELOG(make_span(y.data()+partition_indices[1],y.data()+partition_indices[2]));
  //  }
  //}

  SUBCASE("partition_sort_minimize_imbalance") {
    interval_vector<int> partition_indices = std_e::partition_sort_minimize_imbalance(y,sz_tot,test_comm,0.1);
    //ELOG(partition_indices);
    auto partition_indices_tot = all_reduce(partition_indices.as_base(),MPI_SUM,test_comm);
    if (rk==0) {
      ELOG(partition_indices_tot);
      ELOG(interval_lengths(partition_indices_tot));
    }
  }

}
