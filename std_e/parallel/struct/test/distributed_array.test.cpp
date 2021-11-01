#include "std_e/unit_test/doctest_mpi.hpp"

#include "std_e/log.hpp"
#include "std_e/parallel/struct/distributed_array.hpp"
#include "std_e/algorithm/distribution.hpp"
#include "std_e/execution/execution.hpp"

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

    future gp = create_gather_protocol2(f0,std::move(f1));
    //future gp = create_gather_protocol2(f0,f1);

    future f2 = input_data(tg,a);
    future f_res = get_from_array(gp,f2);

    ELOG(to_dot_format_string(tg));

    SUBCASE("seq") {
      CHECK( execute_seq(f_res) == vector{70,90,20,10,30,110,50} );
    }
    //SUBCASE("async comm") {
    //  thread_pool comm_tp(2);
    //  CHECK( execute_async_comm(f_res,comm_tp) == vector{70,90,20,10,30,110,50} );
    //}
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


  //SUBCASE("gather_from_ranks") {
  //  jagged_array indices = {{2,1},{3,5},{7},{11,9}};

  //  vector<int> local_array(7);
  //  { dist_guard _(a);
  //    gather_from_ranks(a,indices,loc_array);
  //  }

  //  CHECK( loc_array == vector{70,90,20,10,30,110,50} );
  //}
  //SUBCASE("sorted gather") {
  //  jagged_array indices = {{1,2},{3,5},{7},{9,11}};
  //  sorted_gather_protocol gp(indices)

  //  vector<int> local_array(7);
  //  { dist_guard _(a);
  //    gp.get(a,loc_array);
  //  }

  //  CHECK( loc_array == vector{70,90,20,10,30,110,50} );
  //}

  //SUBCASE("gather") {
  //  vector indices = {7,9,2,1,3,11,5};
  //  gather_protocol gp(distri,indices);

  //  vector<int> local_array(7);
  //  { dist_guard _(a);
  //    gp.get(a,loc_array);
  //  }

  //  CHECK( loc_array == vector{70,90,20,10,30,110,50} );
  //}

  //SUBCASE("gather") {
  //  vector<int> local_array(7);

  //  { dist_guard _(a);
  //    gather(a,vector{7,9,2,1,3,11,5},loc_array);
  //  }

  //  CHECK( loc_array == vector{70,90,20,10,30,110,50} );
  //}
}
