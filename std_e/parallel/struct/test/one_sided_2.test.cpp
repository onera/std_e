#include "doctest/extensions/doctest_mpi.h"
#include "std_e/parallel/mpi.hpp"
#include "std_e/interval/interval_sequence.hpp"

#include "std_e/log.hpp"
#include "std_e/parallel/struct/distributed_array_op.hpp"
#include "std_e/algorithm/distribution.hpp"

//#include <chrono>
//#include <thread>
//std::this_thread::sleep_for(std::chrono::milliseconds(x));
using namespace std_e;


//#include <thread>
//#include <new>
//TEST_CASE("thread") {
//  unsigned int n_hw_threads = std::thread::hardware_concurrency();
//  ELOG(n_hw_threads);
//  ELOG(std::hardware_constructive_interference_size);
//  ELOG(std::hardware_destructive_interference_size);
//}


MPI_TEST_CASE("win alloc",2) {
  distribution_vector<g_num> distri = {0,3,7};

  distributed_array<int> a(distri , test_comm);

  {
    window_guard _0(a.win());

    if (rank(test_comm)==0) {
      a.local()[0] = 100;
      a.local()[1] = 1;
      a.local()[2] = 10;
    } else if (rank(test_comm)==1) {
      a.local()[0] = 200;
      a.local()[1] = 2;
      a.local()[2] = 20;
      a.local()[3] = 2000;
    }
  }
  MPI_Barrier(test_comm);

  SUBCASE("scalar get") {
    int n = distri.length();
    std::vector<int> complete_array(n);

    {
      window_guard _(a.win());
      for (int i=0; i<n; ++i) {
        complete_array[i] = get(a,i);
      }
    }

    CHECK(complete_array == std::vector{100,1,10,200,2,20,2000});
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




template<class I>
distribution_vector<I> uniform_distribution(int size, I nb_elts) {
  distribution_vector<I> distrib(size);
  uniform_distribution(begin(distrib),end(distrib),nb_elts);
  return distrib;
}

MPI_TEST_CASE("win alloc 48",48) {
  int n_rank = 48;
  g_num dn_elt = 2;
  auto distri = uniform_distribution(n_rank,n_rank*dn_elt);

  distributed_array<int> a(distri , test_comm);

  // init
  {
    window_guard _(a.win());

    int rk = rank(test_comm);
    std::iota(a.local().begin(),a.local().end(),rk*dn_elt);
  }
  MPI_Barrier(test_comm);

  SUBCASE("scalar get") {
    int n = distri.length();
    std::vector<int> complete_array(n);

    {
      window_guard _(a.win());
      for (int i=0; i<n; ++i) {
        complete_array[i] = rget(a,i);
      }
    }

    MPI_Barrier(test_comm);
    ELOG(complete_array);
    //std::iota(a.local().begin(),a.local().end(),rk*dn_elt);
    //CHECK(complete_array == );
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

