#include "std_e/unit_test/doctest.hpp"
#include "std_e/utils/frozen_flat_map.hpp"

#include <string>

using std::vector;
using std::string;

TEST_CASE("frozen_flat_map") {
  vector<string> strs = {"BBB","AAA","CCC","DDD"};

  SUBCASE("map") {
    vector<int>    ints = { 100 ,  99 ,  88 , 77  };

    std_e::frozen_flat_map<string,int> fm(strs,ints);

    CHECK( fm["BBB"] == 100 );
    CHECK( fm["AAA"] ==  99 );
    CHECK( fm["CCC"] ==  88 );
    CHECK( fm["DDD"] ==  77 );
  }
  SUBCASE("permutation_frozen_flat_map") {
    auto fm = std_e::permutation_frozen_flat_map(strs);
    CHECK( fm["BBB"] == 0 );
    CHECK( fm["AAA"] == 1 );
    CHECK( fm["CCC"] == 2 );
    CHECK( fm["DDD"] == 3 );
  }
}
