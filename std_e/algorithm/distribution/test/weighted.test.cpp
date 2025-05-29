#include "std_e/unit_test/doctest.hpp"
#include "std_e/algorithm/distribution/weighted.hpp"
#include <vector>

using namespace std_e;

TEST_CASE("balanced_distribution") {
  SUBCASE("uniform") {
    int n_slot = 3;
    std::vector<int> sizes = {100,100,100,100,100};
    std::vector<int> weights = {1,1,1,1,1};

    auto [v,v_weighted] = balanced_distribution(n_slot,sizes,weights);
    std_e::interval_vector<int> v_expected = {0,167,334,500};
    std_e::interval_vector<int> v_weighted_expected = {0,167,334,500};
    CHECK( v == v_expected );
    CHECK( v_weighted == v_weighted_expected );

    SUBCASE("numbers_of_elt_in_interval") {
      auto [elts_inf_0,elts_sup_0] = elements_in_interval(v_weighted[0],v_weighted[1],sizes,weights);
      auto [elts_inf_1,elts_sup_1] = elements_in_interval(v_weighted[1],v_weighted[2],sizes,weights);
      auto [elts_inf_2,elts_sup_2] = elements_in_interval(v_weighted[2],v_weighted[3],sizes,weights);
      CHECK( elts_inf_0 == std::vector{  0,  0,  0,  0,  0} );
      CHECK( elts_sup_0 == std::vector{100, 67,  0,  0,  0} );

      CHECK( elts_inf_1 == std::vector{100, 67,  0,  0,  0} );
      CHECK( elts_sup_1 == std::vector{100,100,100, 34,  0} );

      CHECK( elts_inf_2 == std::vector{100,100,100, 34,  0} );
      CHECK( elts_sup_2 == std::vector{100,100,100,100,100} );
    }
    SUBCASE("numbers_of_elt_in_interval - less blocs than slots") {
      int n_slot = 10;
      auto [v,v_weighted] = balanced_distribution(n_slot,sizes,weights);
      auto [elts_inf_0,elts_sup_0] = elements_in_interval(v_weighted[0],v_weighted[ 1],sizes,weights);
      auto [elts_inf_1,elts_sup_1] = elements_in_interval(v_weighted[1],v_weighted[ 2],sizes,weights);
      auto [elts_inf_2,elts_sup_2] = elements_in_interval(v_weighted[2],v_weighted[ 3],sizes,weights);
      auto [elts_inf_3,elts_sup_3] = elements_in_interval(v_weighted[3],v_weighted[ 4],sizes,weights);
      auto [elts_inf_9,elts_sup_9] = elements_in_interval(v_weighted[9],v_weighted[10],sizes,weights);

      CHECK( v          == std_e::interval_vector{0,50,100,150,200,250,300,350,400,450,500} );
      CHECK( v_weighted == std_e::interval_vector{0,50,100,150,200,250,300,350,400,450,500} );

      CHECK( elts_inf_0 == std::vector{  0,  0,  0,  0,  0} );
      CHECK( elts_sup_0 == std::vector{ 50,  0,  0,  0,  0} );

      CHECK( elts_inf_1 == std::vector{ 50,  0,  0,  0,  0} );
      CHECK( elts_sup_1 == std::vector{100,  0,  0,  0,  0} );

      CHECK( elts_inf_2 == std::vector{100,  0,  0,  0,  0} );
      CHECK( elts_sup_2 == std::vector{100, 50,  0,  0,  0} );

      CHECK( elts_inf_3 == std::vector{100, 50,  0,  0,  0} );
      CHECK( elts_sup_3 == std::vector{100,100,  0,  0,  0} );

      CHECK( elts_inf_9 == std::vector{100,100,100,100, 50} );
      CHECK( elts_sup_9 == std::vector{100,100,100,100,100} );
    }
  }

  SUBCASE("non-uniform") {
    int n_slot = 3;
    std::vector<int> sizes = {100,100,100,100,100};
    std::vector<int> weights = {3,5,1,0,2};

    auto [v,v_weighted] = balanced_distribution(n_slot,sizes,weights);
    std_e::interval_vector<int> v_expected = {0,114,187,500};
    std_e::interval_vector<int> v_weighted_expected = {0,/* 3*100 + 5*14 = */ 370,/* 3*100 + 5*(100-14-1) = */735,/* (3+5+1+0+2)*100 = */1100};
    CHECK( v == v_expected );
    CHECK( v_weighted == v_weighted_expected );

    SUBCASE("numbers_of_elt_in_interval") {
      auto [elts_inf_0,elts_sup_0] = elements_in_interval(v_weighted[0],v_weighted[1],sizes,weights);
      auto [elts_inf_1,elts_sup_1] = elements_in_interval(v_weighted[1],v_weighted[2],sizes,weights);
      auto [elts_inf_2,elts_sup_2] = elements_in_interval(v_weighted[2],v_weighted[3],sizes,weights);

      CHECK( elts_inf_0 == std::vector{  0,  0,  0,  0,  0} );
      CHECK( elts_sup_0 == std::vector{100, 14,  0,  0,  0} );

      CHECK( elts_inf_1 == std::vector{100, 14,  0,  0,  0} );
      CHECK( elts_sup_1 == std::vector{100, 87,  0,  0,  0} );

      CHECK( elts_inf_2 == std::vector{100, 87,  0,  0,  0} );
      CHECK( elts_sup_2 == std::vector{100,100,100,100,100} );
    }
  }

  SUBCASE("simple") {
    int n_slot = 2;
    std::vector<int> sizes = {12,2,1,3};
    std::vector<int> weights = {4,3,3,4};

    auto [v,v_weighted] = balanced_distribution(n_slot,sizes,weights);
    CHECK( v == std_e::interval_vector{0,9,18} );
    CHECK( v_weighted == std_e::interval_vector{0,36,69} );
  }

  SUBCASE("zero-weight border cases") {
    SUBCASE("zero at the end") {
      int n_slot = 2;
      std::vector<int> sizes = {1,1,1};
      std::vector<int> weights = {1,1,0};

      auto [v,v_weighted] = balanced_distribution(n_slot,sizes,weights);
      std_e::interval_vector<int> v_expected = {0,1,3};
      std_e::interval_vector<int> v_weighted_expected = {0,1,2};
    }
    SUBCASE("zero at the begining") {
      int n_slot = 2;
      std::vector<int> sizes = {1,1,1};
      std::vector<int> weights = {0,1,1};

      auto [v,v_weighted] = balanced_distribution(n_slot,sizes,weights);
      std_e::interval_vector<int> v_expected = {0,2,3};
      std_e::interval_vector<int> v_weighted_expected = {0,1,2};
    }
  }
}
