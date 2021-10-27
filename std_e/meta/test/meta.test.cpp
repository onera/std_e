#include "std_e/unit_test/doctest.hpp"
#include "std_e/meta/meta.hpp"

#include <vector>
#include <algorithm>
#include <functional>
using std::vector;
using std::tuple;
using std::unique_ptr;
using std::make_unique;

TEST_CASE("reference_wrapper") {
  int  a = 1;
  auto ra = std::ref(a);
  auto cra = std::cref(a);
  // static_assert()
  CHECK( std_e::is_reference_wrapper<decltype(ra)>  == true  );
  CHECK( std_e::is_reference_wrapper<decltype(a)>   == false );
  CHECK( std_e::is_reference_wrapper<decltype(cra)> == true  );
}

TEST_CASE("apply_forward") {
  vector<int> v = {40,60};
  tuple<int, vector<int>&, unique_ptr<int>> t = {10,v,make_unique<int>(1000)};
  auto add_and_modify = [](int&& x, std::vector<int>& y, unique_ptr<int>&& ptr) {
    int res = x+y[0]+y[1]+*ptr;
    y[0] += 2;
    y[1] += 3;
    return res;
  };

  // the second element of the tuple "t" is a reference, so it will be passed by reference to "add_and_modify"
  // the other elements are not references, so they are moved to "add_and_modify"
  int res = std_e::apply_forward(add_and_modify,t);
  // notice that since it compiles, it means that the third element was not copied (but moved) because a unique_ptr is move-only

  CHECK( res == 10 + 40 + 60 + 1000 );
  CHECK( std::get<1>(t) == vector{42,63} ); // the second element itself was modified (not a copy)
}
