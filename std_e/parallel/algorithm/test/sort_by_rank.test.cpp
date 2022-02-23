#include "std_e/log.hpp"
#if __cplusplus > 201703L
#include "std_e/unit_test/doctest_mpi.hpp"

#include "std_e/parallel/algorithm/sort_by_rank.hpp"
#include "std_e/interval/algorithm.hpp"
#include "std_e/unit_test/math.hpp"
#include "std_e/data_structure/multi_range/multi_range.hpp"
#include "std_e/data_structure/block_range/block_range.hpp"

using namespace std_e;
using std::vector;


MPI_TEST_CASE("sort_by_rank - 2 procs - small case",2) {
  int rk = test_rank;

  vector<int> x;
  if (rk == 0) x = {12,10};
  if (rk == 1) x = {10};

  interval_vector<int> rank_indices = std_e::sort_by_rank(x,test_comm);
  MPI_CHECK( 0 , rank_indices == interval_vector{0,1,2} );
  MPI_CHECK( 0 ,            x ==          vector{10,12} );
  MPI_CHECK( 1 , rank_indices == interval_vector{0,1,1} );
  MPI_CHECK( 1 ,            x ==          vector{10} );
}

MPI_TEST_CASE("sort_by_rank - 2 procs",2) {
  int rk = test_rank;

  vector<int> x;
  if (rk == 0) x = {13,11,10,14,0,7,9,6};
  if (rk == 1) x = {12,3,4,8,5,1,2};

  interval_vector<int> rank_indices = std_e::sort_by_rank(x,test_comm);

  // check that `pivot_partition_once` did partition `x` according to the returned indices
  CHECK( is_partitioned_at_indices(x,rank_indices) );

  // Since max_imbalance==0., and the total size is 15,
  // It means the global partition indices are exactly 0,8,15
  // The global partition index is the sum over the ranks of rank_indices
  CHECK( all_reduce(rank_indices.as_base(),MPI_SUM,test_comm) == vector{0,8,15} ); // TODO as_base is ugly!

  // regression testing values
  MPI_CHECK( 0 , rank_indices == interval_vector{0,       3,        8} );
  MPI_CHECK( 0 ,            x ==          vector{0,6,7,9,10,13,14,11} );
  MPI_CHECK( 1 , rank_indices == interval_vector{0,        5,  7} );
  MPI_CHECK( 1 ,            x ==          vector{2,3,4,1,5,8,12} );
}

MPI_TEST_CASE("sort_by_rank - already sorted and balanced",3) {
  int rk = test_rank;

  vector<int> x;
  if (rk == 0) x = {0,1,2,3,4};
  if (rk == 1) x = {5,6,7,8,9};
  if (rk == 2) x = {10,11,12,13,14};

  interval_vector<int> rank_indices = std_e::sort_by_rank(x,test_comm);

  CHECK( is_partitioned_at_indices(x,rank_indices) );

  // Since max_imbalance==0., and the total size is 15,
  // It means the global partition indices are exactly 0,5,10,15
  // The global partition index is the sum over the ranks of rank_indices
  CHECK( all_reduce(rank_indices.as_base(),MPI_SUM,test_comm) == vector{0,5,10,15} ); // TODO as_base is ugly!

  // Note that `x` was already sorted, however it does not stay so because the partition is unstable
  MPI_CHECK( 0, rank_indices == interval_vector{0,       5,5,5} );
  MPI_CHECK( 0,            x ==          vector{0,1,2,3,4} );
  MPI_CHECK( 1, rank_indices == interval_vector{0,0  ,     5,5} );
  MPI_CHECK( 1,            x ==          vector{  5,6,7,8,9} );
  MPI_CHECK( 2, rank_indices == interval_vector{0,0, 0,           5} );
  MPI_CHECK( 2,            x ==          vector{    11,10,12,13,14} );
}

MPI_TEST_CASE("sort_by_rank - already sorted, but imbalance",3) {
  int rk = test_rank;

  vector<int> x;
  if (rk == 0) x = {0,1,2,3,4,5};
  if (rk == 1) x = {6,7,8,9};
  if (rk == 2) x = {10,11,12,13,14};

  interval_vector<int> rank_indices = std_e::sort_by_rank(x,test_comm);

  CHECK( is_partitioned_at_indices(x,rank_indices) );

  // Since max_imbalance==0., and the total size is 15,
  // It means the global partition indices are exactly 0,5,10,15
  // The global partition index is the sum over the ranks of rank_indices
  CHECK( all_reduce(rank_indices.as_base(),MPI_SUM,test_comm) == vector{0,5,10,15} ); // TODO as_base is ugly!

  // Note that `x` was already sorted, however it does not stay so because the partition is unstable
  // Since it was not perfectly balanced, the partition indices reflect the fact that value `5` should move from rank 0 to rank 1
  MPI_CHECK( 0, rank_indices == interval_vector{0,        5,6,6} );
  MPI_CHECK( 0,            x ==          vector{0,1,2,3,4,5} );
  MPI_CHECK( 1, rank_indices == interval_vector{0,0,     4,4} );
  MPI_CHECK( 1,            x ==          vector{  6,7,8,9} );
  MPI_CHECK( 2, rank_indices == interval_vector{0,0, 0,           5} );
  MPI_CHECK( 2,            x ==          vector{    11,10,12,13,14} );
}


//MPI_TEST_CASE("sort_by_rank - cardinal sine function",16) {
//  int sz_tot = 256'000'000;
//MPI_TEST_CASE("sort_by_rank - cardinal sine function",16) {
//  int sz_tot = 1'000'000;
//MPI_TEST_CASE("sort_by_rank - cardinal sine function",4) {
//  int sz_tot = 64'000;
//MPI_TEST_CASE("sort_by_rank - cardinal sine function",4) {
//  int sz_tot = 32'000;
//MPI_TEST_CASE("sort_by_rank - cardinal sine function",16) {
//  int sz_tot = 64000;
//MPI_TEST_CASE("sort_by_rank - cardinal sine function",16) {
//  int sz_tot = 500'000;
//MPI_TEST_CASE("sort_by_rank - cardinal sine function",16) {
//  int sz_tot = 200;
MPI_TEST_CASE("sort_by_rank - cardinal sine function",4) {
  int sz_tot = 200;
  int rk = test_rank;
  int n_rk = test_nb_procs;

  int sz = sz_tot/n_rk;
  interval<double> in = interval_portion({-5.,10.},n_rk,rk); // distribute [-5.,10.) over the procs
  std::vector<double> y = function_result_vector(sinc,sz,in); // take `sz` sample points of the sinc function over domain `in`

  SUBCASE("no imbalance") {
    double max_imbalance = 0.;
    interval_vector<int> rank_indices = std_e::sort_by_rank(y,test_comm,{},{},max_imbalance);

    CHECK( is_partitioned_at_indices(y,rank_indices) );

    auto partition_indices_tot = all_reduce(rank_indices.as_base(),MPI_SUM,test_comm);
    // Note: perfect balance is obtained because for this input, there is no repeated value that falls on a boundary between ranks
    CHECK( partition_indices_tot == vector{0,50,100,150,200} );
  }
  SUBCASE("40 percent imbalance") {
    double max_imbalance = 0.4;
    interval_vector<int> rank_indices = std_e::sort_by_rank(y,test_comm,{},{},max_imbalance);

    CHECK( is_partitioned_at_indices(y,rank_indices) );

    auto partition_indices_tot = all_reduce(rank_indices.as_base(),MPI_SUM,test_comm);

    CHECK( partition_indices_tot == vector{0,59,109,151,200} );
    // NOTE: The imbalance is actually, in this case, appoximately two times better than what asked for: (59-0)/50=18% vs 40%
    //       This is because everything is shifted in the same direction.
    //       If we were unlucky, we could have had e.g. vector{0,41,109,151,200}:
    //         in this case, while the max distance to the perfect position stays the same (==9)
    //         we would have a worse imbalance on rank 1: (151-41)/50=36%
  }
}


// =======================================================
// Degenerate cases
MPI_TEST_CASE("sort_by_rank - empty range",2) {
  vector<int> x;
  interval_vector<int> rank_indices = std_e::sort_by_rank(x,test_comm);
  CHECK( rank_indices == interval_vector{0,0,0} );
}

MPI_TEST_CASE("sort_by_rank - less values than procs",2) {
  SUBCASE("value on proc 0") {
    int rk = test_rank;
    vector<int> x;
    if (rk == 0) x = {10};
    if (rk == 1) x = {};
    interval_vector<int> rank_indices = std_e::sort_by_rank(x,test_comm);
    MPI_CHECK( 0, rank_indices == interval_vector{0,0,1} ); // the value will stay on proc 0
    MPI_CHECK( 0,            x ==          vector{10} );
    MPI_CHECK( 1, rank_indices == interval_vector{0,0,0} );
    MPI_CHECK( 1,            x ==          vector<int>{} );
  }
  SUBCASE("value on proc 1") {
    int rk = test_rank;
    vector<int> x;
    if (rk == 0) x = {};
    if (rk == 1) x = {10};
    interval_vector<int> rank_indices = std_e::sort_by_rank(x,test_comm);
    MPI_CHECK( 0, rank_indices == interval_vector{0,0,0} );
    MPI_CHECK( 0,            x ==          vector<int>{} );
    MPI_CHECK( 1, rank_indices == interval_vector{0,0,1} ); // the value will stay on proc 1
    MPI_CHECK( 1,            x ==          vector{10} );
  }
}
MPI_TEST_CASE("sort_by_rank - less values than procs, but still need to sort",3) {
  SUBCASE("all on proc 0") {
    int rk = test_rank;
    vector<int> x;
    if (rk == 0) x = {11,10};
    if (rk == 1) x = {};
    if (rk == 2) x = {};
    interval_vector<int> rank_indices = std_e::sort_by_rank(x,test_comm);
    MPI_CHECK( 0, rank_indices == interval_vector{0,0,1,2} ); // 10 goes to proc 1, 11 goes to proc 2
    MPI_CHECK( 0,            x ==          vector{10,11} );
    MPI_CHECK( 1, rank_indices == interval_vector{0,0,0,0} );
    MPI_CHECK( 1,            x ==          vector<int>{} );
    MPI_CHECK( 2, rank_indices == interval_vector{0,0,0,0} );
    MPI_CHECK( 2,            x ==          vector<int>{} );
  }

  SUBCASE("on two procs") {
    int rk = test_rank;
    vector<int> x;
    if (rk == 0) x = {11};
    if (rk == 1) x = {};
    if (rk == 2) x = {10};
    interval_vector<int> rank_indices = std_e::sort_by_rank(x,test_comm);
    MPI_CHECK( 0, rank_indices == interval_vector{0,0,0,1} ); // 11 goes to proc 2
    MPI_CHECK( 0,            x ==          vector{11} );
    MPI_CHECK( 1, rank_indices == interval_vector{0,0,0,0} );
    MPI_CHECK( 1,            x ==          vector<int>{} );
    MPI_CHECK( 2, rank_indices == interval_vector{0,0,1,1} ); // 10 goes to proc 1
    MPI_CHECK( 2,            x ==          vector{10} );
  }
}

MPI_TEST_CASE("sort_by_rank - repeated values",3) {
  int rk = test_rank;

  SUBCASE("one value") {
    vector<int> x;
    if (rk == 0) x = {10};
    if (rk == 1) x = {10};
    if (rk == 2) x = {10};

    interval_vector<int> rank_indices = std_e::sort_by_rank(x,test_comm);
    CHECK( rank_indices == interval_vector{0,0,1,1} ); // all values will end up on proc 1
    CHECK(            x ==          vector{10} );
  }
  SUBCASE("one value, more copies") {
    vector<int> x;
    if (rk == 0) x = {10,10,10};
    if (rk == 1) x = {};
    if (rk == 2) x = {10,10};

    interval_vector<int> rank_indices = std_e::sort_by_rank(x,test_comm);
    MPI_CHECK( 0 , rank_indices == interval_vector{0,0,3,3} );
    MPI_CHECK( 0 ,            x ==          vector{10,10,10} );
    MPI_CHECK( 1 , rank_indices == interval_vector{0,0,0,0} );
    MPI_CHECK( 1 ,            x ==          vector<int>{} );
    MPI_CHECK( 2 , rank_indices == interval_vector{0,0,2,2} );
    MPI_CHECK( 2 ,            x ==          vector{10,10} );
  }
  SUBCASE("two values") {
    vector<int> x;
    if (rk == 0) x = {12,10};
    if (rk == 1) x = {10};
    if (rk == 2) x = {10};

    interval_vector<int> rank_indices = std_e::sort_by_rank(x,test_comm);
    MPI_CHECK( 0 , rank_indices == interval_vector{0,1,1,2} );
    MPI_CHECK( 0 ,            x ==          vector{10,12} );
    MPI_CHECK( 1 , rank_indices == interval_vector{0,1,1,1} );
    MPI_CHECK( 1 ,            x ==          vector{10} );
    MPI_CHECK( 2 , rank_indices == interval_vector{0,1,1,1} );
    MPI_CHECK( 2 ,            x ==          vector{10} );
  }
}


MPI_TEST_CASE("sort_by_rank - with and without indirect projector",2) {
  int rk = test_rank;

  vector<double> x;
  if (rk == 0) x  = { 1.0, 10.1, 12.5, 0.6};
  if (rk == 1) x  = { 3.7, 12.1,  1.2};

  SUBCASE("direct") {
    interval_vector<int> rank_indices = std_e::sort_by_rank(x,test_comm);

    MPI_CHECK( 0, rank_indices == interval_vector{0,2,4} );
    MPI_CHECK( 1, rank_indices == interval_vector{0,2,3} );

    CHECK( is_partitioned_at_indices(x,rank_indices) );

    // regression testing values
    MPI_CHECK( 0, x == vector{0.6,1.0,  10.1,12.5} );
    MPI_CHECK( 1, x == vector{3.7,1.2,  12.1} );
  }

  SUBCASE("indirect") {
    // Same thing as the previous SUBCASE, but instead of
    // permuting the values of `x` directly, we permute their indices.
    // For that, we pass the indices `is` to `sort_by_rank`,
    // and we also pass a custom projector that project `is` onto `x`
    vector<int> is;
    if (rk == 0) is = {   0,    1,    2,   3};
    if (rk == 1) is = {   4,    5,    6}; // Note that we are using a global numbering,
                                          // so the indices of rank 1 start at x<rank_0>.size()

    int offset;
    if (rk == 0) offset = 0;
    if (rk == 1) offset = 4;
    auto proj = [&x,offset](int i){ return x[i-offset]; }; // We need the offset because of the global numbering
    interval_vector<int> rank_indices = std_e::sort_by_rank(is,test_comm,proj);

    // same as previous SUBCASE
    MPI_CHECK( 0, rank_indices == interval_vector{0,2,4} );
    MPI_CHECK( 1, rank_indices == interval_vector{0,2,3} );

    // apply permutation
    vector<double> x_perm(x.size());
    for (size_t i=0; i<x.size(); ++i) {
      x_perm[i] = x[is[i]-offset];
    }
    // test the partitioning was done through the indirection
    CHECK( is_partitioned_at_indices(x_perm,rank_indices) );

    // regression testing values: same as previous SUBCASE
    MPI_CHECK( 0, x_perm == vector{0.6,1.0,  10.1,12.5} );
    MPI_CHECK( 1, x_perm == vector{3.7,1.2,  12.1} );
  }
}


MPI_TEST_CASE("sort_by_rank - with and without indirect projector - 3 procs",3) {
  int rk = test_rank;

  vector<int> x;
  if (rk == 0) x = {13,11,10,14,0};
  if (rk == 1) x = {12,3,4,8};
  if (rk == 2) x = {7,9,6,5,1,2};

  SUBCASE("direct") {
    interval_vector<int> rank_indices = std_e::sort_by_rank(x,test_comm);

    CHECK( is_partitioned_at_indices(x,rank_indices) );

    // Since max_imbalance==0., and the total size is 15,
    // It means the global partition indices are exactly 0,5,10,15
    // The global partition index is the sum over the ranks of rank_indices
    CHECK( all_reduce(rank_indices.as_base(),MPI_SUM,test_comm) == vector{0,5,10,15} ); // TODO as_base is ugly!


    // regression testing values
    MPI_CHECK( 0, rank_indices == interval_vector{0,1,1,         5} );
    MPI_CHECK( 0,            x ==          vector{0,  10,11,14,13} );
    MPI_CHECK( 1, rank_indices == interval_vector{0,  2, 3,4} );
    MPI_CHECK( 1,            x ==          vector{4,3,8,12} );
    MPI_CHECK( 2, rank_indices == interval_vector{0,  2,     6,6} );
    MPI_CHECK( 2,            x ==          vector{2,1,6,5,7,9} );
  }
  SUBCASE("indirect") {
    int offset = ex_scan(x.size(),MPI_SUM,0,test_comm);

    vector<int> is(x.size());
    std::iota(begin(is),end(is),offset);

    auto proj = [&x,offset](int i){ return x[i-offset]; };

    interval_vector<int> rank_indices = std_e::sort_by_rank(is,test_comm,proj);

    // Same as previous SUBCASE
    CHECK( all_reduce(rank_indices.as_base(),MPI_SUM,test_comm) == vector{0,5,10,15} ); // TODO as_base is ugly!

    // apply permutation
    vector<int> x_perm(x.size());
    for (size_t i=0; i<x.size(); ++i) {
      x_perm[i] = x[is[i]-offset];
    }
    // test the partitioning was done through the indirection
    CHECK( is_partitioned_at_indices(x_perm,rank_indices) );

    // regression testing values: same as previous SUBCASE
    MPI_CHECK( 0, rank_indices == interval_vector{0,1,1,         5} );
    MPI_CHECK( 0,       x_perm ==          vector{0,  10,11,14,13} );
    MPI_CHECK( 1, rank_indices == interval_vector{0,  2, 3,4} );
    MPI_CHECK( 1,       x_perm ==          vector{4,3,8,12} );
    MPI_CHECK( 2, rank_indices == interval_vector{0,  2,     6,6} );
    MPI_CHECK( 2,       x_perm ==          vector{2,1,6,5,7,9} );
  }
}


MPI_TEST_CASE("sort_by_rank - multi_range - 2 procs",2) {
  int rk = test_rank;

  vector<int> x;
  vector<int> y;
  if (rk == 0) x = {13,11,10,14, 0, 7 ,9, 6};
  if (rk == 0) y = { 0, 1, 2, 3, 4, 5 ,6, 7};
  if (rk == 1) x = {12, 3, 4, 8, 5, 1, 2};
  if (rk == 1) y = { 8, 9,10,11,12,13,14};

  auto mr = view_as_multi_range(x,y);
  auto proj = [](const auto& x){ return get<0>(x); };
  interval_vector<int> rank_indices = std_e::sort_by_rank(mr,test_comm,proj);

  // check that `pivot_partition_once` did partition `x` according to the returned indices
  CHECK( is_partitioned_at_indices(x,rank_indices) );

  // Since max_imbalance==0., and the total size is 15,
  // It means the global partition indices are exactly 0,8,15
  // The global partition index is the sum over the ranks of rank_indices
  CHECK( all_reduce(rank_indices.as_base(),MPI_SUM,test_comm) == vector{0,8,15} ); // TODO as_base is ugly!

  // regression testing values
  MPI_CHECK( 0 , rank_indices == interval_vector{0,       3,        8} );
  MPI_CHECK( 0 ,            x ==          vector{ 0, 6, 7, 9,10,13,14,11} );
  MPI_CHECK( 0 ,            y ==          vector{ 4, 7, 5, 6, 2, 0, 3, 1} );
  MPI_CHECK( 1 , rank_indices == interval_vector{0,        5,  7} );
  MPI_CHECK( 1 ,            x ==          vector{ 2, 3, 4, 1, 5, 8,12} );
  MPI_CHECK( 1 ,            y ==          vector{14, 9,10,13,12,11, 8} );
}

MPI_TEST_CASE("sort_by_rank - block_range - 2 procs",2) {
  // The two procs have the same data (two blocks of 3 elements)
  // The first  block of rank 0 and the first  block of rank 1 will go to rank 0
  // The second block of rank 0 and the second block of rank 1 will go to rank 1
  // TODO use different data to make it clear what is going on
  std::vector<int> x = {10,4,5,  6,3,12};
  auto xb = view_as_block_range<3>(x);

  std_e::sort_by_rank(xb,test_comm);
  // TODO check idx
  CHECK( x == std::vector{6,3,12,  10,4,5} );
}

MPI_TEST_CASE("sort_by_rank - block_range - 2 procs",2) {
  // case with proxy reference of mixed proxy reference/real references
  // same as above, both rank have the same data
  // TODO use different data to make it clear what is going on
  std::vector<int> x = {10,4,5,  6,3,12};
  std::vector<int> y = {100   ,  200};

  using block_range_type = std_e::block_range<std::vector<int>&,3>;
  block_range_type xb = std_e::view_as_block_range<3>(x);

  using span_type = std_e::span<int>;
  span_type ys (y.data(),y.size());

  std_e::multi_range2<block_range_type,span_type> t(xb,ys);

  constexpr auto proj = [](const auto& x) -> decltype(auto) { return get<0>(x); };
  std_e::sort_by_rank(t,test_comm,proj);
  // TODO check idx
  CHECK( x == std::vector{6,3,12,  10,4,5} );
  CHECK( y == std::vector{200   ,  100} );
}
#endif // __cplusplus > 201703L
