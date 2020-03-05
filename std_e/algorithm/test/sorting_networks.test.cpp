#include "doctest/doctest.h"

#include "std_e/algorithm/sorting_networks.hpp"
#include <vector>


using namespace std;
using namespace std_e;


TEST_CASE("sorting networks") {
  SUBCASE("size 2") {
    vector<int> expected_v = {0,1};

    vector<int> v_0 = {0,1};
    vector<int> v_1 = {1,0};

    sorting_network<2>::sort(begin(v_0));
    sorting_network<2>::sort(begin(v_1));

    CHECK( v_0 == expected_v );
    CHECK( v_1 == expected_v );

  }
  SUBCASE("size 3") {
    vector<int> expected_v = {0,1,2};

    vector<int> v_0 = {0,1,2};
    vector<int> v_1 = {1,0,2};
    vector<int> v_2 = {1,2,0};
    vector<int> v_3 = {2,1,0};
    vector<int> v_4 = {0,2,1};
    vector<int> v_5 = {2,0,1};

    sorting_network<3>::sort(begin(v_0));
    sorting_network<3>::sort(begin(v_1));
    sorting_network<3>::sort(begin(v_2));
    sorting_network<3>::sort(begin(v_3));
    sorting_network<3>::sort(begin(v_4));
    sorting_network<3>::sort(begin(v_5));

    CHECK( v_0 == expected_v );
    CHECK( v_1 == expected_v );
    CHECK( v_2 == expected_v );
    CHECK( v_3 == expected_v );
    CHECK( v_4 == expected_v );
    CHECK( v_5 == expected_v );
  }
  SUBCASE("size 4") {
    vector<int> expected_v = {0,1,2,3};

    vector<int> v_00 = {0,1,2,3};
    vector<int> v_01 = {1,0,2,3};
    vector<int> v_02 = {1,2,0,3};
    vector<int> v_03 = {2,1,0,3};
    vector<int> v_04 = {0,2,1,3};
    vector<int> v_05 = {2,0,1,3};
    vector<int> v_10 = {3,1,2,0};
    vector<int> v_11 = {1,3,2,0};
    vector<int> v_12 = {1,2,3,0};
    vector<int> v_13 = {2,1,3,0};
    vector<int> v_14 = {3,2,1,0};
    vector<int> v_15 = {2,3,1,0};
    vector<int> v_20 = {0,3,2,1};
    vector<int> v_21 = {3,0,2,1};
    vector<int> v_22 = {3,2,0,1};
    vector<int> v_23 = {2,3,0,1};
    vector<int> v_24 = {0,2,3,1};
    vector<int> v_25 = {2,0,3,1};
    vector<int> v_30 = {0,1,3,2};
    vector<int> v_31 = {1,0,3,2};
    vector<int> v_32 = {1,3,0,2};
    vector<int> v_33 = {3,1,0,2};
    vector<int> v_34 = {0,3,1,2};
    vector<int> v_35 = {3,0,1,2};

    sorting_network<4>::sort(begin(v_00));
    sorting_network<4>::sort(begin(v_01));
    sorting_network<4>::sort(begin(v_02));
    sorting_network<4>::sort(begin(v_03));
    sorting_network<4>::sort(begin(v_04));
    sorting_network<4>::sort(begin(v_05));
    sorting_network<4>::sort(begin(v_10));
    sorting_network<4>::sort(begin(v_11));
    sorting_network<4>::sort(begin(v_12));
    sorting_network<4>::sort(begin(v_13));
    sorting_network<4>::sort(begin(v_14));
    sorting_network<4>::sort(begin(v_15));
    sorting_network<4>::sort(begin(v_20));
    sorting_network<4>::sort(begin(v_21));
    sorting_network<4>::sort(begin(v_22));
    sorting_network<4>::sort(begin(v_23));
    sorting_network<4>::sort(begin(v_24));
    sorting_network<4>::sort(begin(v_25));
    sorting_network<4>::sort(begin(v_30));
    sorting_network<4>::sort(begin(v_31));
    sorting_network<4>::sort(begin(v_32));
    sorting_network<4>::sort(begin(v_33));
    sorting_network<4>::sort(begin(v_34));
    sorting_network<4>::sort(begin(v_35));

    CHECK( v_00 == expected_v );
    CHECK( v_01 == expected_v );
    CHECK( v_02 == expected_v );
    CHECK( v_03 == expected_v );
    CHECK( v_04 == expected_v );
    CHECK( v_05 == expected_v );
    CHECK( v_10 == expected_v );
    CHECK( v_11 == expected_v );
    CHECK( v_12 == expected_v );
    CHECK( v_13 == expected_v );
    CHECK( v_14 == expected_v );
    CHECK( v_15 == expected_v );
    CHECK( v_20 == expected_v );
    CHECK( v_21 == expected_v );
    CHECK( v_22 == expected_v );
    CHECK( v_23 == expected_v );
    CHECK( v_24 == expected_v );
    CHECK( v_25 == expected_v );
    CHECK( v_30 == expected_v );
    CHECK( v_31 == expected_v );
    CHECK( v_32 == expected_v );
    CHECK( v_33 == expected_v );
    CHECK( v_34 == expected_v );
    CHECK( v_35 == expected_v );
  }
}
