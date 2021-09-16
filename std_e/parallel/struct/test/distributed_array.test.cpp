#include "std_e/unit_test/doctest_mpi.hpp"

#include "std_e/log.hpp"
#include "std_e/parallel/struct/distributed_array.hpp"
#include "std_e/algorithm/distribution.hpp"

using namespace std_e;
using std::vector;


MPI_TEST_CASE("load distributed array - multiple nodes",48) { // 48 is supposed to be enough to force multiple nodes
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
      load(a,i,complete_array[i]);
    }
  }

  std::vector<int> expected_complete_array(n_rank*dn_elt);
  std::iota(expected_complete_array.begin(),expected_complete_array.end(),0);
  CHECK(complete_array == expected_complete_array);
}


MPI_TEST_CASE("load distributed array - small unit test",4) {
  // init data
  int rk = test_rank;
  int n_rank = test_nb_procs;
  g_num dn_elt = 3;
  auto distri = uniform_distribution(n_rank,n_rank*dn_elt);


  // init distributed array
  dist_array<int> a(distri , test_comm);
  { dist_guard _(a);
    if (rk==0) a.local() = std::vector{ 0, 10, 20};
    if (rk==1) a.local() = std::vector{30, 40, 50};
    if (rk==2) a.local() = std::vector{60, 70, 80};
    if (rk==3) a.local() = std::vector{90,100,110};
  }

  // comm and test
  SUBCASE("scalar load") {
    int n = distri.length();
    std::vector<int> complete_array(n,-1);

    { dist_guard _(a);
      for (int i=0; i<n; ++i) {
        load(a,i,complete_array[i]);
      }
    }

    CHECK( complete_array == vector{0,10,20, 30,40,50, 60,70,80, 90,100,110} );
  }

  SUBCASE("contiguous load") {
    vector<int> blk_1(3);
    vector<int> blk_2(2);
    vector<int> blk_3(2);

    { dist_guard _(a);
      // ask 3 values from rank 1
      load(a,1,0,blk_1);
      //
      // ask 2 first values from rank 2
      load(a,2,0,blk_2);

      // ask 2 values from rank 3, and begin at the second one
      load(a,3,1,blk_3);
    }

    CHECK( blk_1 == vector{30,40,50} );
    CHECK( blk_2 == vector{60,70}    );
    CHECK( blk_3 == vector{100,110}  );
  }

  SUBCASE("indexed load") {
    vector<int> blk_0(2);
    vector<int> blk_2(3);

    { dist_guard _(a);
      // ask values 0 and 2 of rank 0
      load(a,0,vector{0,2},blk_0);

      // ask values 0, 2 and 1 of rank 2
      load(a,2,vector{0,2,1},blk_2);

    }

    CHECK( blk_0 == vector{0,20} );
    CHECK( blk_2 == vector{60,80,70} );
  }

  SUBCASE("gather") {
    //SUBCASE("asking contiguous on several ranks") {
    //  int first = 3;
    //  int last = 7;
    //  auto blk = std_e::make_span(complete_array.data()+first, complete_array.data()+last);

    //  { dist_guard _(a);
    //    gather(a,first,blk);
    //  }

    //  std::vector<int> expected_array(n,-1);
    //  std::iota(begin(expected_array)+first,end(expected_array)+last,first);
    //  CHECK(complete_array == expected_array);
    //}

    //SUBCASE("non-contiguous on several ranks") {
    //  { dist_guard _(a);
    //    for (int i=0; i<n; i+=dn_elt) {
    //      load(a,i,blk);
    //    }
    //  }

    //  CHECK(complete_array == expected_complete_array);
    //}
  }

  //ELOG(i3);
  //// 2. test multiple values
  //if (rank==0) {
  //  std::vector<int> v = remote_values_from_block(d_array,1,{1,2,0,3});
  //  assert( v.size() == 4 );
  //  assert( v[0] == 2 );
  //  assert( v[1] == 20 );
  //  assert( v[2] == 200 );
  //  assert( v[3] == 2000 );
  //}

  //// 3. test multiple value multiple distant
  //if (rank==0) {
  //  std::vector<int> v = remote_values_from_blocks(d_array,{0,1},{vector{1,2,0},vector{1,2,0,3}});
  //  assert( v.size() == 7 );
  //  assert( v[0] == 1 );
  //  assert( v[1] == 10 );
  //  assert( v[2] == 100 );
  //  assert( v[3] == 2 );
  //  assert( v[4] == 20 );
  //  assert( v[5] == 200 );
  //  assert( v[6] == 2000 );
  //}

  //// 4. block_to_part
  //if (rank==0) {
  //  std::vector<int> v = block_to_part(d_array,distri,{3,1,0,5,2});
  //  assert( v.size() == 5 );
  //  std::cout << "v[0] = " << v[0] << "\n";
  //  std::cout << "v[1] = " << v[1] << "\n";
  //  std::cout << "v[2] = " << v[2] << "\n";
  //  std::cout << "v[3] = " << v[3] << "\n";
  //  std::cout << "v[4] = " << v[4] << "\n";
  //  assert( v[0] == 200 );
  //  assert( v[1] == 1   );
  //  assert( v[2] == 100 );
  //  assert( v[3] == 20  );
  //  assert( v[4] == 10  );
  //}
}
