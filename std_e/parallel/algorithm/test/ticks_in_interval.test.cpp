#if __cplusplus > 201703L
#include "std_e/unit_test/doctest_mpi.hpp"

#include "std_e/parallel/algorithm/ticks_in_interval.hpp"

using namespace std;
using namespace std_e;

// TODO `compute_interval_containing_ticks` is wrong in `sort_by_rank` because found interval do not shift `indices`
// => use `compute_interval_containing_ticks2` and delete `compute_interval_containing_ticks`
MPI_TEST_CASE("compute_interval_containing_ticks - 12 procs",12) {
  vector<int> obj_ticks = {          4,8,12, 16,19,22,      25,28,31,      34,37};
  //                      {0, 0,0,0,0,       16,   22,24,24,         32,32, 36  ,40}
  //                                 0        3  4  5        6              9 10
  interval_vector<int> indices;
  if (test_rank== 0) indices = {0,0,0,0,0,3,5,5,5,8,8,8,8};
  if (test_rank== 1) indices = {0,0,0,0,0,3,4,4,4,7,7,8,8};
  if (test_rank== 2) indices = {0,0,0,0,0,3,3,4,4,5,5,5,8};
  if (test_rank== 3) indices = {0,0,0,0,0,5,5,5,5,6,6,7,8};
  if (test_rank== 4) indices = {0,0,0,0,0,1,1,1,1,1,1,1,1};
  if (test_rank== 5) indices = {0,0,0,0,0,1,1,1,1,1,1,1,1};
  if (test_rank== 6) indices = {0,0,0,0,0,0,1,1,1,1,1,1,1};
  if (test_rank== 7) indices = {0,0,0,0,0,0,1,1,1,1,1,1,1};
  if (test_rank== 8) indices = {0,0,0,0,0,0,1,1,1,1,1,1,1};
  if (test_rank== 9) indices = {0,0,0,0,0,0,0,1,1,1,1,1,1};
  if (test_rank==10) indices = {0,0,0,0,0,0,0,0,0,0,0,1,1};
  if (test_rank==11) indices = {0,0,0,0,0,0,0,0,0,0,0,1,1};

  auto new_sub_ins = compute_intervals_containing_ticks(obj_ticks,indices,0,test_comm);
//Rank  0: sub_ins = {[0,3):3:0,[3,5):1:4,[5,8):3:6,[8,8):1:9,[8,8):1:10}
//Rank  1: sub_ins = {[0,3):3:0,[3,4):1:4,[4,7):3:6,[7,8):1:9,[8,8):1:10}
//Rank  2: sub_ins = {[0,3):3:0,[3,3):1:4,[4,5):3:6,[5,5):1:9,[5,8):1:10}
//Rank  3: sub_ins = {[0,5):3:0,[5,5):1:4,[5,6):3:6,[6,7):1:9,[7,8):1:10}
//Rank  4: sub_ins = {[0,1):3:0,[1,1):1:4,[1,1):3:6,[1,1):1:9,[1,1):1:10}
//Rank  5: sub_ins = {[0,1):3:0,[1,1):1:4,[1,1):3:6,[1,1):1:9,[1,1):1:10}
//Rank  6: sub_ins = {[0,0):3:0,[0,1):1:4,[1,1):3:6,[1,1):1:9,[1,1):1:10}
//Rank  7: sub_ins = {[0,0):3:0,[0,1):1:4,[1,1):3:6,[1,1):1:9,[1,1):1:10}
//Rank  8: sub_ins = {[0,0):3:0,[0,1):1:4,[1,1):3:6,[1,1):1:9,[1,1):1:10}
//Rank  9: sub_ins = {[0,0):3:0,[0,0):1:4,[1,1):3:6,[1,1):1:9,[1,1):1:10}
//Rank 10: sub_ins = {[0,0):3:0,[0,0):1:4,[0,0):3:6,[0,1):1:9,[1,1):1:10}
//Rank 11: sub_ins = {[0,0):3:0,[0,0):1:4,[0,0):3:6,[0,1):1:9,[1,1):1:10}

  //MPI_CHECK(  0 , indices == std_e::interval_vector{0,0,0,3,0,0,0,0,0,0,0,0,0} );
  //MPI_CHECK(  1 , indices == std_e::interval_vector{0,0,0,3,0,0,0,0,0,0,0,0,0} );
  //MPI_CHECK(  2 , indices == std_e::interval_vector{0,0,0,3,0,0,0,0,0,0,0,0,0} );
  //MPI_CHECK(  3 , indices == std_e::interval_vector{0,0,0,5,0,0,0,0,0,0,0,0,0} );
  //MPI_CHECK(  4 , indices == std_e::interval_vector{0,0,0,1,0,0,0,0,0,0,0,0,0} );
  //MPI_CHECK(  5 , indices == std_e::interval_vector{0,0,0,1,0,0,0,0,0,0,0,0,0} );
  //MPI_CHECK(  6 , indices == std_e::interval_vector{0,0,0,0,0,0,0,0,0,0,0,0,0} );
  //MPI_CHECK(  7 , indices == std_e::interval_vector{0,0,0,0,0,0,0,0,0,0,0,0,0} );
  //MPI_CHECK(  8 , indices == std_e::interval_vector{0,0,0,0,0,0,0,0,0,0,0,0,0} );
  //MPI_CHECK(  9 , indices == std_e::interval_vector{0,0,0,0,0,0,0,0,0,0,0,0,0} );
  //MPI_CHECK( 10 , indices == std_e::interval_vector{0,0,0,0,0,0,0,0,0,0,0,0,0} );
  //MPI_CHECK( 11 , indices == std_e::interval_vector{0,0,0,0,0,0,0,0,0,0,0,0,0} );
}


MPI_TEST_CASE("compute_interval_containing_ticks2 - 11 procs",11) {
  vector<int> obj_ticks = {4,8,12,16};
  int max_interval_tick_shift = 0;
  int position_offset = 0;
  interval_vector<int> indices(test_nb_procs,0);

  interval_vector<int> partition_indices_sub;
  if (test_rank== 0) partition_indices_sub = {0,0,0,0,0,0,3,3,4,4};
  if (test_rank== 1) partition_indices_sub = {0,0,0,0,0,0,3,3,4,4};
  if (test_rank== 2) partition_indices_sub = {0,0,0,0,0,0,3,3,3,3};
  if (test_rank== 3) partition_indices_sub = {0,0,0,0,0,0,5,5,5,5};
  if (test_rank== 4) partition_indices_sub = {0,0,0,0,0,0,1,1,1,1};
  if (test_rank== 5) partition_indices_sub = {0,0,0,0,0,0,1,1,1,1};
  if (test_rank== 6) partition_indices_sub = {0,0,0,0,0,0,0,0,1,1};
  if (test_rank== 7) partition_indices_sub = {0,0,0,0,0,0,0,0,1,1};
  if (test_rank== 8) partition_indices_sub = {0,0,0,0,0,0,0,0,0,0};
  if (test_rank== 9) partition_indices_sub = {0,0,0,0,0,0,0,0,0,0};
  if (test_rank==10) partition_indices_sub = {0,0,0,0,0,0,0,0,0,0};

  auto new_sub_ins = compute_interval_containing_ticks2(
    obj_ticks,partition_indices_sub,max_interval_tick_shift,
    position_offset,indices,test_comm
  );

  MPI_CHECK(  0 , indices == std_e::interval_vector{0,0,0,3,3,0,0,0,0,0,0,0} );
  MPI_CHECK(  1 , indices == std_e::interval_vector{0,0,0,3,3,0,0,0,0,0,0,0} );
  MPI_CHECK(  2 , indices == std_e::interval_vector{0,0,0,3,3,0,0,0,0,0,0,0} );
  MPI_CHECK(  3 , indices == std_e::interval_vector{0,0,0,5,5,0,0,0,0,0,0,0} );
  MPI_CHECK(  4 , indices == std_e::interval_vector{0,0,0,1,1,0,0,0,0,0,0,0} );
  MPI_CHECK(  5 , indices == std_e::interval_vector{0,0,0,1,1,0,0,0,0,0,0,0} );
  MPI_CHECK(  6 , indices == std_e::interval_vector{0,0,0,0,0,0,0,0,0,0,0,0} );
  MPI_CHECK(  7 , indices == std_e::interval_vector{0,0,0,0,0,0,0,0,0,0,0,0} );
  MPI_CHECK(  8 , indices == std_e::interval_vector{0,0,0,0,0,0,0,0,0,0,0,0} );
  MPI_CHECK(  9 , indices == std_e::interval_vector{0,0,0,0,0,0,0,0,0,0,0,0} );
  MPI_CHECK( 10 , indices == std_e::interval_vector{0,0,0,0,0,0,0,0,0,0,0,0} );
}
MPI_TEST_CASE("compute_interval_containing_ticks2 - 12 procs",12) {
  vector<int> obj_ticks = {4,8,12};
  int max_interval_tick_shift = 0;
  int position_offset = 0;
  interval_vector<int> indices(test_nb_procs,0);

  interval_vector<int> partition_indices_sub;
  if (test_rank== 0) partition_indices_sub = {0,0,0,0,3,3,3,3};
  if (test_rank== 1) partition_indices_sub = {0,0,0,0,3,3,3,3};
  if (test_rank== 2) partition_indices_sub = {0,0,0,0,3,3,3,3};
  if (test_rank== 3) partition_indices_sub = {0,0,0,0,5,5,5,5};
  if (test_rank== 4) partition_indices_sub = {0,0,0,0,1,1,1,1};
  if (test_rank== 5) partition_indices_sub = {0,0,0,0,1,1,1,1};
  if (test_rank== 6) partition_indices_sub = {0,0,0,0,0,0,0,0};
  if (test_rank== 7) partition_indices_sub = {0,0,0,0,0,0,0,0};
  if (test_rank== 8) partition_indices_sub = {0,0,0,0,0,0,0,0};
  if (test_rank== 9) partition_indices_sub = {0,0,0,0,0,0,0,0};
  if (test_rank==10) partition_indices_sub = {0,0,0,0,0,0,0,0};
  if (test_rank==11) partition_indices_sub = {0,0,0,0,0,0,0,0};

  auto new_sub_ins = compute_interval_containing_ticks2(
    obj_ticks,partition_indices_sub,max_interval_tick_shift,
    position_offset,indices,test_comm
  );

  MPI_CHECK(  0 , indices == std_e::interval_vector{0,0,0,3,0,0,0,0,0,0,0,0,0} );
  MPI_CHECK(  1 , indices == std_e::interval_vector{0,0,0,3,0,0,0,0,0,0,0,0,0} );
  MPI_CHECK(  2 , indices == std_e::interval_vector{0,0,0,3,0,0,0,0,0,0,0,0,0} );
  MPI_CHECK(  3 , indices == std_e::interval_vector{0,0,0,5,0,0,0,0,0,0,0,0,0} );
  MPI_CHECK(  4 , indices == std_e::interval_vector{0,0,0,1,0,0,0,0,0,0,0,0,0} );
  MPI_CHECK(  5 , indices == std_e::interval_vector{0,0,0,1,0,0,0,0,0,0,0,0,0} );
  MPI_CHECK(  6 , indices == std_e::interval_vector{0,0,0,0,0,0,0,0,0,0,0,0,0} );
  MPI_CHECK(  7 , indices == std_e::interval_vector{0,0,0,0,0,0,0,0,0,0,0,0,0} );
  MPI_CHECK(  8 , indices == std_e::interval_vector{0,0,0,0,0,0,0,0,0,0,0,0,0} );
  MPI_CHECK(  9 , indices == std_e::interval_vector{0,0,0,0,0,0,0,0,0,0,0,0,0} );
  MPI_CHECK( 10 , indices == std_e::interval_vector{0,0,0,0,0,0,0,0,0,0,0,0,0} );
  MPI_CHECK( 11 , indices == std_e::interval_vector{0,0,0,0,0,0,0,0,0,0,0,0,0} );
}
#endif // __cplusplus > 201703L
