#include "std_e/unit_test/doctest.hpp"


#include "std_e/base/lift.hpp"
#include <string>
#include <vector>
#include <algorithm>


using namespace std;


auto f__test_lift(int& i)    -> void { ++i; }
auto f__test_lift(string& s) -> void { s += "a"; }

TEST_CASE("LIFT") {
  vector<int> vi = {1,2,3};
  vector<string> vs = {"a","b","c"};

//for_each(vi.begin(),vi.end(),     f__test_lift); // does not work (language defect)
  for_each(vi.begin(),vi.end(),LIFT(f__test_lift)); // works!
  CHECK( vi == vector{2,3,4} );

//for_each(vs.begin(),vs.end(),     f__test_lift); // does not work (language defect)
  for_each(vs.begin(),vs.end(),LIFT(f__test_lift)); // works!
  CHECK( vs == (vector<string>{"aa","ba","ca"}) );
}
