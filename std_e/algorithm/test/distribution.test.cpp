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
    std::vector<int> v(1+n_slot);

    std::vector<int> sizes = {100,100,100,100,100};
    std::vector<int> weights = {1,1,1,1,1};

    //distribution_weighted_by_blocks(begin(v),end(v),begin(sizes),end(sizes),begin(weights));
    distribution_weighted_by_blocks(v.data(),v.data()+4,begin(sizes),end(sizes),begin(weights));
    std::vector<int> v_expected = {0,167,334,500};
    CHECK( v == v_expected );
  }

  SUBCASE("non-uniform") {
    int n_slot = 3;
    std::vector<int> v(1+n_slot);

    std::vector<int> sizes = {100,100,100,100,100};
    std::vector<int> weights = {3,5,1,0,2};

    distribution_weighted_by_blocks(v.data(),v.data()+1+n_slot,begin(sizes),end(sizes),begin(weights));
    std::vector<int> v_expected = {0,113,186,500};
    CHECK( v == v_expected );
  }
}
