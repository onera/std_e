#include "doctest/doctest.h"
#include "std_e/utils/frozen_flat_map.hpp"

#include <string>

using std::vector;
using std::string;

TEST_CASE("frozen_flat_map") {
  vector<string> strs = {"BBB","AAA","CCC","DDD"};
  vector<int>    ints = { 100 ,  99 ,  88 , 77  };

  std_e::frozen_flat_map<string,int> fm(strs,ints);

  CHECK( fm["BBB"] == 100 );
  CHECK( fm["AAA"] ==  99 );
  CHECK( fm["CCC"] ==  88 );
  CHECK( fm["DDD"] ==  77 );
}
