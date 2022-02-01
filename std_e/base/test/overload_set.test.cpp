#include "std_e/unit_test/doctest.hpp"


#include "std_e/base/overload_set.hpp"
#include <string>
#include <vector>
#include <algorithm>


using namespace std;


namespace {
// [Sphinx Doc] overload_set {
auto f_overload_set(int& i)    -> void { ++i; }
auto f_overload_set(string& s) -> void { s += "a"; }

TEST_CASE("overload set macro") {
  vector<int> vi = {1,2,3};
  vector<string> vs = {"a","b","c"};

//for_each(vi.begin(),vi.end(),                   f_overload_set); // does not work (language defect)
  for_each(vi.begin(),vi.end(),STD_E_OVERLOAD_SET(f_overload_set)); // works!
  CHECK( vi == vector{2,3,4} );

//for_each(vs.begin(),vs.end(),                   f_overload_set); // does not work (language defect)
  for_each(vs.begin(),vs.end(),STD_E_OVERLOAD_SET(f_overload_set)); // works!
  CHECK( vs == (vector<string>{"aa","ba","ca"}) );
}
// [Sphinx Doc] overload_set }
} // anonymous
