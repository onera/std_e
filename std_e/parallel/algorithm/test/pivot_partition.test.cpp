#include "std_e/unit_test/doctest_mpi.hpp"

#include "std_e/parallel/algorithm/pivot_partition.hpp"
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

//MPI_TEST_CASE("parallel pivot_partition",16) {
//  int sz_tot = 256'000'000;
MPI_TEST_CASE("parallel pivot_partition - cardinal sine function",4) {
  int sz_tot = 200;
  int rk = test_rank;
  int n_rk = test_nb_procs;

  int sz = sz_tot/n_rk;
  interval<double> in = interval_portion({-5.,10.},n_rk,rk); // distribute [-5.,10.) over the procs
  std::vector<double> y = function_result_vector(sinc,sz,in); // take `sz` sample points of the sinc function over domain `in`

  SUBCASE("pivot_partition_once") {
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
    auto partition_indices_tot = all_reduce(partition_indices.as_base(),MPI_SUM,test_comm);
    if (rk==0) {
      //ELOG(interval_lengths(partition_indices));
      ELOG(partition_indices_tot);
      ELOG(interval_lengths(partition_indices_tot));
      //ELOG(make_span(y.data()+partition_indices[1],y.data()+partition_indices[2]));
    }
  }

  SUBCASE("pivot_partition_minimize_imbalance") {
    double max_imbalance = 0.1;
    interval_vector<int> partition_indices = std_e::pivot_partition_minimize_imbalance(y,sz_tot,test_comm,max_imbalance);

    CHECK( is_partitioned_at_indices(y,partition_indices) );

    auto partition_indices_tot = all_reduce(partition_indices.as_base(),MPI_SUM,test_comm);
    auto lens = interval_lengths(partition_indices_tot);
    int optimal_len = sz;
    for (int i=0; i<int(lens.size())-1; ++i) {
      CHECK( std::abs(lens[i]-optimal_len)/double(optimal_len) < max_imbalance );
    }

    if (rk==0) {
      ELOG(partition_indices_tot);
      ELOG(lens);
    }
  }
}

MPI_TEST_CASE("parallel pivot_partition",3) {
  int sz_tot = 15;
  int rk = test_rank;
  int n_rk = test_nb_procs;

  std::vector<int> x;
  //if (rk == 0) x = {0,1,2,3,4};
  //if (rk == 1) x = {5,6,7,8,9};
  //if (rk == 2) x = {10,11,12,13,14};
  if (rk == 0) x = {13,11,10,14,0};
  if (rk == 1) x = {12,3,4,8};
  if (rk == 2) x = {7,9,6,5,1,2};

  double max_imbalance = 0.5;
  //interval_vector<int> partition_indices = std_e::pivot_partition_minimize_imbalance(x,sz_tot,test_comm,max_imbalance);
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
