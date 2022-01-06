#include "std_e/unit_test/doctest_mpi.hpp"


#include "std_e/parallel/algorithm/median_of_3.hpp"


using namespace std_e;


MPI_TEST_CASE("median_of_3_sample",3) {
  int rk = test_rank;

  SUBCASE("regular, already sorted") {
    std::vector<int> x;
    if (rk == 0) x = { 0, 1, 2, 3, 4, 5};
    if (rk == 1) x = { 6, 7, 8, 9,10,11};
    if (rk == 2) x = {12,13,14,15,16,17};

    std::vector<int> sample = std_e::median_of_3_sample(x,test_comm);

    // Note: The exact numbers in the result are here for non-regression mainly.
    //       They are difficult to interpret for a "tutorial" unit test:
    //         We can just see that the numbers are coherent
    //         But it could be other numbers
    //            -> re-write the unit test better if you know how to do it
    CHECK( sample == std::vector{4,11} );
  }

  SUBCASE("irregular") {
    std::vector<int> x;
    if (rk == 0) x = {13,11,10,14,0};
    if (rk == 1) x = {12,3,4,8};
    if (rk == 2) x = {7,9,6,5,1,2};

    std::vector<int> sample = std_e::median_of_3_sample(x,test_comm);

    // Note: same remark as above
    CHECK( sample == std::vector{3,7} );
  }

  SUBCASE("smallest number of values") {
    std::vector<int> x;
    if (rk == 0) x = {5};
    if (rk == 1) x = {};
    if (rk == 2) x = {2};

    std::vector<int> sample = std_e::median_of_3_sample(x,test_comm);

    CHECK( sample == std::vector{2,5} );
  }
  SUBCASE("few values") {
    std::vector<int> x;
    if (rk == 0) x = {5,1,6};
    if (rk == 1) x = {};
    if (rk == 2) x = {2,3};

    std::vector<int> sample = std_e::median_of_3_sample(x,test_comm);

    CHECK( sample == std::vector{2,5} );
  }
}
