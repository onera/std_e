#if __cplusplus > 201703L


#include "std_e/unit_test/doctest_mpi.hpp"

#include "std_e/log.hpp"
#include "std_e/parallel/struct/distributed_array.hpp"
#include "std_e/algorithm/distribution.hpp"
#include "std_e/execution/execution.hpp"
#include "std_e/logging/time_logger.hpp" // TODO

using namespace std_e;
using std::vector;


MPI_TEST_CASE("load distributed array - multiple compute nodes",48) { // 48 is supposed to be enough to force multiple nodes
  // init data
  int n_rank = test_nb_procs;
  g_num dn_elt = 2;
  auto distri = uniform_distribution(n_rank,n_rank*dn_elt);
  int n = distri.length();

  // init distributed array
  dist_array<int> a(distri , test_comm);
  { dist_guard _(a);

    int rk = rank(test_comm);
    std::iota(a.local().begin(),a.local().end(),rk*dn_elt);
  }

  // load data
  std::vector<int> complete_array(n,-1);
  { dist_guard _(a);
    for (int i=0; i<n; ++i) {
      load(a,distri,i,complete_array[i]);
    }
  }

  std::vector<int> expected_complete_array(n_rank*dn_elt);
  std::iota(expected_complete_array.begin(),expected_complete_array.end(),0);
  CHECK(complete_array == expected_complete_array);
}


MPI_TEST_CASE("distributed array - load",4) {
  g_num dn_elt = 3;
  auto distri = uniform_distribution(test_nb_procs,test_nb_procs*dn_elt);

  // init distributed array
  dist_array<int> a(distri, test_comm);
  { dist_guard _(a);
    if (test_rank==0) a.local() = std::vector{ 0, 10, 20};
    if (test_rank==1) a.local() = std::vector{30, 40, 50};
    if (test_rank==2) a.local() = std::vector{60, 70, 80};
    if (test_rank==3) a.local() = std::vector{90,100,110};
  }

  // comm and test
  int n = distri.length();
  std::vector<int> complete_array(n,-1);

  { dist_guard _(a);
    for (int i=0; i<n; ++i) {
      load(a,distri,i,complete_array[i]);
    }
  }

  CHECK( complete_array == vector{0,10,20, 30,40,50, 60,70,80, 90,100,110} );
}


MPI_TEST_CASE("distributed array - get",4) {
  g_num dn_elt = 3;
  auto distri = uniform_distribution(test_nb_procs,test_nb_procs*dn_elt);

  // init distributed array
  dist_array<int> a(distri , test_comm);
  { dist_guard _(a);
    if (test_rank==0) a.local() = std::vector{ 0, 10, 20};
    if (test_rank==1) a.local() = std::vector{30, 40, 50};
    if (test_rank==2) a.local() = std::vector{60, 70, 80};
    if (test_rank==3) a.local() = std::vector{90,100,110};
  }

  SUBCASE("scalar") {
    int scalar_0(3);
    int scalar_1(2);

    { dist_guard _(a);
      // ask value 2 from rank 0
      get_scalar(a,0,2,scalar_0);
      // ask value 1 from rank 3
      get_scalar(a,3,1,scalar_1);
    }

    CHECK( scalar_0 == 20 );
    CHECK( scalar_1 == 100);
  }

  SUBCASE("contiguous") {
    vector<int> loc_array_1(3);
    vector<int> loc_array_2(2);
    vector<int> loc_array_3(2);

    { dist_guard _(a);
      // ask 3 values from rank 1
      get_contiguous(a,1,0,loc_array_1);
      //
      // ask 2 first values from rank 2
      get_contiguous(a,2,0,loc_array_2);

      // ask 2 values from rank 3, and begin at the second one
      get_contiguous(a,3,1,loc_array_3);
    }

    CHECK( loc_array_1 == vector{30,40,50} );
    CHECK( loc_array_2 == vector{60,70}    );
    CHECK( loc_array_3 == vector{100,110}  );
  }

  SUBCASE("indexed") {
    vector<int> loc_array_0(2);
    vector<int> loc_array_2(3);

    { dist_guard _(a);
      // ask values 0 and 2 of rank 0
      get_indexed(a,0,vector{0,2},loc_array_0);

      // ask values 0, 2 and 1 of rank 2
      get_indexed(a,2,vector{0,2,1},loc_array_2);
    }

    CHECK( loc_array_0 == vector{0,20} );
    CHECK( loc_array_2 == vector{60,80,70} );
  }

  SUBCASE("gather_from_ranks") {
    jagged_vector<int> indices = {{2,1},{0,2},{1},{2,0}};

    vector<int> local_array(7);
    { dist_guard _(a);
      gather_from_ranks(a,indices,local_array);
    }

    CHECK( local_array == vector{20,10,30,50,70,110,90} );
  }

  SUBCASE("gather_sorted") {
    vector indices = {1,2,  3,5,  7,  9,11};

    vector<int> local_array(7);
    { dist_guard _(a);
      gather_sorted(a,distri,indices,local_array);
    }

    CHECK( local_array == vector{10,20,  30,50,  70,  90,110} );
  }

  SUBCASE("gather with protocol") {
    task_graph tg;

    future f0 = input_data(tg,distri);
    future f1 = input_data(tg,vector{7,9,2,1,3,11,5});

    future gp = create_exchange_protocol(f0,std::move(f1));

    future f2 = input_data(tg,a);
    future f_res = gather(gp,f2);

    SUBCASE("seq") {
      CHECK( execute_seq(f_res) == vector{70,90,20,10,30,110,50} );
    }
    SUBCASE("async comm") {
      thread_pool comm_tp(2);
      CHECK( execute_async_comm(f_res,comm_tp) == vector{70,90,20,10,30,110,50} );
    }
  }

  SUBCASE("gather with protocol - seq") {
    vector indices = {7,9,2,1,3,11,5};

    int type_sz = sizeof(typename decltype(a)::value_type);
    auto p = create_gather_protocol(distri,indices,type_sz);

    vector<int> local_array(7);
    { dist_guard _(a);
      get_protocol_indexed(a,p.protocols_by_rank,local_array);
    }

    inv_permute(local_array,p.new_to_old);
    CHECK( local_array == vector{70,90,20,10,30,110,50} );
  }

  SUBCASE("gather") {
    vector indices = {7,9,2,1,3,11,5};

    task_graph tg;
    future f0 = input_data(tg,a);
    future f1 = input_data(tg,distri);
    future f2 = input_data(tg,std::move(indices));

    future f_res = gather(f0,f1,f2);

    SUBCASE("seq") {
      CHECK( execute_seq(f_res) == vector{70,90,20,10,30,110,50} );
    }
    SUBCASE("async comm") {
      thread_pool comm_tp(2);
      CHECK( execute_async_comm(f_res,comm_tp) == vector{70,90,20,10,30,110,50} );
    }
  }

  SUBCASE("gather - seq") {
    CHECK( gather(a,distri,vector{7,9,2,1,3,11,5}) == vector{70,90,20,10,30,110,50} );
  }
}

MPI_TEST_CASE("distributed jagged array - gather",4) {
  distribution_vector<g_num> distri = {0,4,6,6,8};

  // init distributed array
  interval_vector<int> indices;
  if (test_rank==0) indices = interval_vector<int>{0,2,5,5,9};
  if (test_rank==1) indices = interval_vector<int>{0,1,3};
  if (test_rank==2) indices = interval_vector<int>{0};
  if (test_rank==3) indices = interval_vector<int>{0,2,3};

  dist_jagged_array<double> a(std::move(indices),test_comm);
  { dist_guard _(a.values_dist_array());
    if (test_rank==0) a.values() = std::vector{0.,0.1,  1.,1.1,1.2,  /*nothing*/  3.,3.1,3.2,3.3};
    if (test_rank==1) a.values() = std::vector{4.,  5.,5.1};
    if (test_rank==2) a.values() = std::vector<double>{};
    if (test_rank==3) a.values() = std::vector{6.,6.1,  7.};
  }

  SUBCASE("gather with protocol") {
    task_graph tg;

    future f0 = input_data(tg,distri);
    future f1 = input_data(tg,vector{7,6,1,2,5});

    future gp = create_exchange_protocol(f0,std::move(f1));

    future f2 = input_data(tg,a);
    future f_res = gather(gp,f2);

    SUBCASE("seq") {
      auto _ = std_e::stdout_time_logger("gather with protocol seq");
      CHECK( execute_seq(f_res) == jagged_vector<double>{{7.},{6.,6.1},{1.,1.1,1.2},{},{5.,5.1}} );
    }
    SUBCASE("async comm") {
      thread_pool comm_tp(2);
      auto _ = std_e::stdout_time_logger("gather with protocol async comm");
      CHECK( execute_async_comm(f_res,comm_tp) == jagged_vector<double>{{7.},{6.,6.1},{1.,1.1,1.2},{},{5.,5.1}} );
    }
  }
}


MPI_TEST_CASE("distributed array - scatter",4) {
  g_num dn_elt = 3;
  auto distri = uniform_distribution(test_nb_procs,test_nb_procs*dn_elt);

  dist_array<double> a(distri , test_comm);

  vector<int> indices;
  vector<double> values;
  if (test_rank==0) { indices = {5,2,11,0,1,4,8}; values = {5.,2.,11.,0.,1.,4.,8.}; }
  if (test_rank==1) { indices = {7}             ; values = {7.}                   ; }
  if (test_rank==2) { indices = {}              ; values = {}                     ; }
  if (test_rank==3) { indices = {10,3,6,9}      ; values = {10.,3.,6.,9.}         ; }

  SUBCASE("future") {

    task_graph tg;
    future f0 = input_data(tg,a);
    future f1 = input_data(tg,distri);
    future f2 = input_data(tg,std::move(indices));
    future f3 = input_data(tg,values);

    future f_res = scatter(f0,f1,f2,f3);

    SUBCASE("seq") {
      execute_seq(f_res);
      MPI_Barrier(test_comm); // TODO move inside scatter ?
      MPI_CHECK(0, a.local() == vector{0., 1., 2.} );
      MPI_CHECK(1, a.local() == vector{3., 4., 5.} );
      MPI_CHECK(2, a.local() == vector{6., 7., 8.} );
      MPI_CHECK(3, a.local() == vector{9.,10.,11.} );
    }
    SUBCASE("async comm") {
      thread_pool comm_tp(2);
      execute_async_comm(f_res,comm_tp);
      MPI_Barrier(test_comm);
      MPI_CHECK(0, a.local() == vector{0., 1., 2.} );
      MPI_CHECK(1, a.local() == vector{3., 4., 5.} );
      MPI_CHECK(2, a.local() == vector{6., 7., 8.} );
      MPI_CHECK(3, a.local() == vector{9.,10.,11.} );
    }
  }

  SUBCASE("eager") {
    scatter(a,distri,std::move(indices),values);
    MPI_CHECK(0, a.local() == vector{0., 1., 2.} );
    MPI_CHECK(1, a.local() == vector{3., 4., 5.} );
    MPI_CHECK(2, a.local() == vector{6., 7., 8.} );
    MPI_CHECK(3, a.local() == vector{9.,10.,11.} );
  }
}


#endif // C++20
