#include "std_e/unit_test/doctest.hpp"
#include "std_e/algorithm/distribution/uniform.hpp"
#include "std_e/parallel/struct/distribution.hpp"
#include <vector>

using namespace std_e;


TEST_CASE("uniform") {
  int sz_tot = 38;
  int n_interval = 5;

  SUBCASE("uniform_distribution_point") {
    CHECK( uniform_distribution_point(sz_tot,n_interval,0) ==  0 );
    CHECK( uniform_distribution_point(sz_tot,n_interval,1) ==  8 ); // + 8
    CHECK( uniform_distribution_point(sz_tot,n_interval,2) == 16 ); // + 8
    CHECK( uniform_distribution_point(sz_tot,n_interval,3) == 24 ); // + 8
    CHECK( uniform_distribution_point(sz_tot,n_interval,4) == 31 ); // + 7
    CHECK( uniform_distribution_point(sz_tot,n_interval,5) == 38 ); // + 7
  }

  SUBCASE("uniform_distribution") {
    // TODO the arguments are in the wrong order !!!
    CHECK( uniform_distribution(n_interval,sz_tot) == interval_vector{0,8,16,24,31,38} );
  }

  SUBCASE("uniform_distribution_") {
    // TODO the arguments are in the wrong order !!!
    CHECK( uniform_distribution_exclude_ends(n_interval,sz_tot) == interval_vector{8,16,24,31} );
  }
}


TEST_CASE("uniform_distribution") {
  SUBCASE("empty remainder") {
    int n_slot = 3;
    std::vector<int> v(1+n_slot);

    uniform_distribution(begin(v),end(v),100,15);
  // distributing 15 over 3 slots gives 5 + 5 + 5
  //                                    |   |   |
  //                                    v   v   v
    std::vector<int> v_expected = {100,105,110,115};
    CHECK( v == v_expected );
  }

  SUBCASE("non-empty remainder") {
    int n_slot = 3;
    std::vector<int> v(1+n_slot);

    uniform_distribution(begin(v),end(v),100,17);

  // distributing 17 over 3 slots gives 6 + 6 + 5
  //                                    |   |   |
  //                                    v   v   v
    std::vector<int> v_expected = {100,106,112,117};
    CHECK( v == v_expected );
  }

  SUBCASE("no slots") {
    int n_slot = 0;
    std::vector<int> v(1+n_slot);

    uniform_distribution(begin(v),end(v),100,0);

  // distributing  0 over 0 slots gives nothing
  //                                    |
  //                                    v
    std::vector<int> v_expected = {100};
    CHECK( v == v_expected );
  }

  SUBCASE("double specialisation") {
    int n_slot = 4;
    std::vector<double> v(1+n_slot);
    double ming = 0. ;
    double maxg = 10.;
    auto interval_size = maxg-ming;

    uniform_distribution(begin(v),end(v),ming,interval_size);
    std::vector<double> v_expected = {0.,2.5,5.,7.5,10.};
    CHECK( v == v_expected );
  }

  SUBCASE("double specialisation example 2 ") {
    int n_slot = 1;
    std::vector<double> v(1+n_slot);
    double ming = -14. ;
    double maxg =  18.;

    auto interval_size = maxg-ming;

    uniform_distribution(begin(v),end(v),ming,interval_size);
    std::vector<double> v_expected = {-14., 18.};
    CHECK( v == v_expected );
  }
}
