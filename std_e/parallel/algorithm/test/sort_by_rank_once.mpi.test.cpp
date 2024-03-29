#if __cplusplus > 201703L
#include "std_e/unit_test/doctest_mpi.hpp"

#include "std_e/parallel/algorithm/sort_by_rank_once.hpp"
#include "std_e/interval/algorithm.hpp"
#include "std_e/unit_test/math.hpp"


using namespace std;
using namespace std_e;


MPI_TEST_CASE("parallel sort_by_rank_once - 2 procs",2) {
  int rk = test_rank;

  std::vector<int> x;
  if (rk == 0) x = {13,11,10,14,0,7,9,6};
  if (rk == 1) x = {12,3,4,8,5,1,2};

  interval_vector<int> rank_indices = std_e::sort_by_rank_once(x,test_comm);

  // check that `sort_by_rank_once` did partition `x` according to the returned indices
  CHECK( is_partitioned_at_indices(x,rank_indices) );

  // With n_rank==2, `sort_by_rank_once` needs only one pivot,
  // and the pivot choice algorithm is then a regular median_of_3 pivot.
  // So the 3 values used for the pivot are the first, middle and last,
  // that is: 13, 6 and 2. So the pivot is 6

  // If we partition at pivot==6 we have
  //   For rank 0
  MPI_CHECK( 0 , x == vector{0,  11,10,14,13,7,9,6} );
  MPI_CHECK( 0 , rank_indices == interval_vector{0, 1, 8} );

  //   For rank 1
  MPI_CHECK( 1 , x == vector{2,3,4,1,5,  8,12} );
  MPI_CHECK( 1 , rank_indices == interval_vector{0, 5, 7} );
}


MPI_TEST_CASE("parallel sort_by_rank_once - 3 procs",3) {
  int rk = test_rank;
  vector<int> x;
  if (rk == 0) x = {13,11,10,14,0};
  if (rk == 1) x = {12,3,4,8};
  if (rk == 2) x = {7,9,6,5,1,2};

  interval_vector<int> rank_indices = std_e::sort_by_rank_once(x,test_comm);
  MPI_CHECK( 0 , rank_indices == interval_vector{0, 1, 3, 5} );
  MPI_CHECK( 1 , rank_indices == interval_vector{0, 2, 4, 4} );
  MPI_CHECK( 2 , rank_indices == interval_vector{0, 2, 6, 6} );
  // Total                                            0, 5,13,15

  // So the global partition of [0,15) is now [0,1,2,3,4 | 5,6,,7,8,9,10,11,12 | 13,14]
  CHECK( is_partitioned_at_indices(x,rank_indices) );
  MPI_CHECK( 0 , x == vector{0,  11,10,14,13} );
  MPI_CHECK( 1 , x == vector{4,3,  12,8} );
  MPI_CHECK( 2 , x == vector{2,1,  6,5,9,7} );
}

MPI_TEST_CASE("parallel sort_by_rank_once - cardinal sine function",4) {
  int sz_tot = 200;
  int rk = test_rank;
  int n_rk = test_nb_procs;

  int sz = sz_tot/n_rk;
  interval<double> x = interval_portion({-5.,10.},n_rk,rk); // distribute [-5.,10.) over the procs
  vector<double> y = function_result_vector(sinc,sz,x); // take `sz` sample points of the sinc function over domain `x`

  interval_vector<int> rank_indices = std_e::sort_by_rank_once(y,test_comm);
  CHECK( rank_indices.size() == n_rk+1 );

  // test that the array `y` is indeed partitioned
  CHECK( is_partitioned_at_indices(y,rank_indices) );

  // regression testing values
  MPI_CHECK( 0, rank_indices == interval_vector{0,23,25,34,50} );
  MPI_CHECK( 1, rank_indices == interval_vector{0, 0, 0, 0,50} );
  MPI_CHECK( 2, rank_indices == interval_vector{0,36,41,49,50} );
  MPI_CHECK( 3, rank_indices == interval_vector{0, 0, 9,50,50} );
}


MPI_TEST_CASE("parallel sort_by_rank_once - indirect projector",2) {
  int rk = test_rank;

  vector<double> x;
  vector<int> is;
  if (rk == 0) x  = {12.5, 10.1, 1.0, 0.6};
  if (rk == 0) is = {   0,    1,   2,   3};
  if (rk == 1) x  = { 3.7, 12.1, 5.2};
  if (rk == 1) is = {   4,    5,   6};

  int offset;
  if (rk == 0) offset = 0;
  if (rk == 1) offset = 4;
  auto proj = [&x,offset](int i){ return x[i-offset]; };
  interval_vector<int> rank_indices = std_e::sort_by_rank_once(is,test_comm,proj);

  MPI_CHECK( 0, rank_indices == interval_vector{0,2,4} );
  MPI_CHECK( 1, rank_indices == interval_vector{0,1,3} );

  // apply permutation
  vector<double> x_perm(x.size());
  for (size_t i=0; i<x.size(); ++i) {
    x_perm[i] = x[is[i]-offset];
  }
  // test the partitioning was done through the indirection
  CHECK( is_partitioned_at_indices(x_perm,rank_indices) );

  // regression testing values
  MPI_CHECK( 0, x_perm == vector{0.6,1.0,  10.1,12.5} );
  MPI_CHECK( 1, x_perm == vector{3.7,  12.1,5.2} );
}
#endif // __cplusplus > 201703L
