#include "std_e/unit_test/doctest.hpp"


#include "std_e/base/lift.hpp"
#include <string>
#include <vector>
#include <algorithm>


using namespace std;


auto f__test_lift(int& i) { ++i; }
auto f__test_lift(string& s) { s += "a"; }

TEST_CASE("LIFT") {
  vector<int> v = {1,2,3};

//for_each(v.begin(),v.end(),     f__test_lift); // does not work (language defect)
  for_each(v.begin(),v.end(),LIFT(f__test_lift)); // works!

  CHECK( v == (vector<int>{2,3,4}) );
}
