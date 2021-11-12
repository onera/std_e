#include "std_e/unit_test/doctest.hpp"
#include "std_e/algorithm/distribution.hpp"
#include <vector>

using namespace std_e;

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


TEST_CASE("distribution_weighted_by_blocks") {
  SUBCASE("uniform") {
    int n_slot = 3;
    std::vector<int> sizes = {100,100,100,100,100};
    std::vector<int> weights = {1,1,1,1,1};

    auto [v,v_weighted] = distribution_weighted_by_blocks(n_slot,sizes,weights);
    std::vector<int> v_expected = {0,167,334,500};
    std::vector<int> v_weighted_expected = {0,167,334,500};
    CHECK( v == v_expected );
    CHECK( v_weighted == v_weighted_expected );

    SUBCASE("numbers_of_elt_in_interval") {
      std::vector<int> n_elts_0 = numbers_of_elt_in_interval(v_weighted[0],v_weighted[1],sizes,weights);
      std::vector<int> n_elts_1 = numbers_of_elt_in_interval(v_weighted[1],v_weighted[2],sizes,weights);
      std::vector<int> n_elts_2 = numbers_of_elt_in_interval(v_weighted[2],v_weighted[3],sizes,weights);
      CHECK( n_elts_0 == std::vector{100, 67,  0,  0,  0} );
      CHECK( n_elts_1 == std::vector{  0, 33,100, 34,  0} );
      CHECK( n_elts_2 == std::vector{  0,  0,  0, 66,100} );
    }
  }

  SUBCASE("non-uniform") {
    int n_slot = 3;
    std::vector<int> sizes = {100,100,100,100,100};
    std::vector<int> weights = {3,5,1,0,2};

    auto [v,v_weighted] = distribution_weighted_by_blocks(n_slot,sizes,weights);
    std::vector<int> v_expected = {0,113,186,500};
    std::vector<int> v_weighted_expected = {0,/* 3*100 + 5*13 = */ 365,/* 3*100 + 5*(100-13-1) = */730,/* (3+5+1+0+2)*100 = */1100};
    CHECK( v == v_expected );
    CHECK( v_weighted == v_weighted_expected );

    SUBCASE("numbers_of_elt_in_interval") {
      std::vector<int> n_elts_0 = numbers_of_elt_in_interval(v_weighted[0],v_weighted[1],sizes,weights);
      std::vector<int> n_elts_1 = numbers_of_elt_in_interval(v_weighted[1],v_weighted[2],sizes,weights);
      std::vector<int> n_elts_2 = numbers_of_elt_in_interval(v_weighted[2],v_weighted[3],sizes,weights);
      CHECK( n_elts_0 == std::vector{100, 13,  0,  0,  0} );
      CHECK( n_elts_1 == std::vector{  0, 73,  0,  0,  0} );
      CHECK( n_elts_2 == std::vector{  0, 14,100,100,100} );
    }
  }
}
