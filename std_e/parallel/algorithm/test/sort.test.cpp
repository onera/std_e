#if __cplusplus > 201703L
#include "std_e/unit_test/doctest_mpi.hpp"

#include "std_e/parallel/algorithm/sort.hpp"
#include "std_e/interval/algorithm.hpp"
#include "std_e/unit_test/math.hpp"

using std::vector;

MPI_TEST_CASE("parallel sort - with and without indirect projector - 3 procs",3) {
  int rk = test_rank;

  vector<int> x;
  if (rk == 0) x = {13,11,10,14,0};
  if (rk == 1) x = {12,3,4,8};
  if (rk == 2) x = {7,9,6,5,1,2};

  SUBCASE("direct") {
    auto [x_sorted,distri] = std_e::sort(x,test_comm);

    CHECK( distri == vector{0,5,10,15} );

    MPI_CHECK( 0 , x_sorted == vector{ 0, 1, 2, 3, 4} );
    MPI_CHECK( 1 , x_sorted == vector{ 5, 6, 7, 8, 9} );
    MPI_CHECK( 2 , x_sorted == vector{10,11,12,13,14} );
  }

  SUBCASE("indirect - one range") { // same case as before, same effect as direct sort
                                    //   generally slower (as here, with value_type==int)
                                    //   may be faster if the value_type is expansive to move
    auto [x_sorted,distri] = std_e::indirect_sort(std::tie(x),test_comm);

    CHECK( distri == vector{0,5,10,15} );

    MPI_CHECK( 0 , x_sorted == vector{ 0, 1, 2, 3, 4} );
    MPI_CHECK( 1 , x_sorted == vector{ 5, 6, 7, 8, 9} );
    MPI_CHECK( 2 , x_sorted == vector{10,11,12,13,14} );
  }

  SUBCASE("indirect - more ranges") {
    vector<double> x_d;
    if (rk == 0) x_d = {0.5, 1.0, 1.5, 2.0, 2.5};
    if (rk == 1) x_d = {3.0, 3.5, 4.0, 4.5};
    if (rk == 2) x_d = {5.0, 5.5, 6.0, 6.5, 7.0, 7.5};

    auto proj = [](int i, double){ return i; }; // Only sort by looking at the first argument
                                                // In our case, it means that we are sorting range `x`
                                                // Range `x_d` is not used for determining the sort permutation, but will be permuted

    auto [x_sorted,x_d_sorted,distri] = std_e::indirect_sort(std::tie(x,x_d),test_comm,proj);

    CHECK( distri == vector{0,5,10,15} );

    MPI_CHECK( 0 , x_sorted   == vector{  0,   1,   2,   3,   4} );
    MPI_CHECK( 0 , x_d_sorted == vector{2.5, 7.0, 7.5, 3.5, 4.0} );
    MPI_CHECK( 1 , x_sorted   == vector{  5,   6,   7,   8,   9} );
    MPI_CHECK( 1 , x_d_sorted == vector{6.5, 6.0, 5.0, 4.5, 5.5} );
    MPI_CHECK( 2 , x_sorted   == vector{ 10,  11,  12,  13,  14} );
    MPI_CHECK( 2 , x_d_sorted == vector{1.5, 1.0, 3.0, 0.5, 2.0} );
  }
}
#endif // __cplusplus > 201703L
