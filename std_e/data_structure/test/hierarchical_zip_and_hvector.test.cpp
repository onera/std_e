#include "doctest/doctest.h"
#include "doctest/doctest_utils.hpp"
#include "std_e/data_structure/hierarchical_zip.hpp"
#include "std_e/data_structure/heterogenous_vector.hpp"

#include <string>

using std::string;
using std::vector;
using std_e::hvector;

struct S0 {
  int i;
};
struct S1 {
  double d;
};
auto to_string(const S0& x) { return std::to_string(    x.i ); }
auto to_string(const S1& x) { return std::to_string(int(x.d)); }
auto compare_to(const S0& x, int i) { return     x.i  == i; }
auto compare_to(const S1& x, int i) { return int(x.d) == i; }

struct S0_names {
  string i;
};
struct S1_names {
  string d;
};
auto set_str_val(S0_names& x, const string& s) { x.i = s; }
auto set_str_val(S1_names& x, const string& s) { x.d = s; }

auto to_string(const S0_names& x) { return x.i; }
auto to_string(const S1_names& x) { return x.d; }

// struct fundamental_hierarchy {
//   std_e::hvector<S0> level1;
// };

// struct optional_hierarchy_1 {
//   std_e::hvector<S0_names> level1;
// };



  // fundamental_hierarchy fh = { {{1, 3.14}, {4, -1.} };
  // optional_hierarchy_1  oh = {};

  // auto s3_with_names = std_e::hzip_view(s3,s3n);
  // SUBCASE("hzip of hvector ") {
  //   for_each_element(s2_with_names_view, [&](auto& x, auto& y){
  //       // Do something
  //   });
  // }


TEST_CASE("hzip with hvector") {
  hvector<S0,S1>             hv  = { vector{S0{10},S0{11},S0{12}} , vector{S1{13.},S1{14.}} };
  hvector<S0_names,S1_names> hvn = { vector<S0_names>(3)          , vector<S1_names>(2)     };

  auto hv_with_names = std_e::hzip_view(hv,hvn);

  auto p = [](const auto& x){ return compare_to(x,13); };
  auto f = [](auto& hiera_zip) {
    string nb_as_str = to_string(std_e::get<0>(hiera_zip));
    set_str_val(std_e::get<1>(hiera_zip) , nb_as_str);
  };
  find_fundamental_type_apply_all(hv_with_names,p,f);

  CHECK( std_e::get<0>(hvn)[0].i == ""   );
  CHECK( std_e::get<0>(hvn)[1].i == ""   );
  CHECK( std_e::get<0>(hvn)[2].i == ""   );
  CHECK( std_e::get<1>(hvn)[0].d == "13" );
  CHECK( std_e::get<1>(hvn)[1].d == ""   );

  SUBCASE("for_each_elements hzip with hvector ") {

    auto g = [&](auto& x1, auto& x2){
    // auto g = [&](auto& x1){
      //std::cout << __PRETTY_FUNCTION__ << std::endl;
      //std::cout << to_string(x1) << " <-> " << to_string(x2) << std::endl;
      // std::cout << " x1.i " << x1.i << " <-> " << " x2.i " << x2.i << std::endl;
      // assert( x1.size() == x2.size() );
      // CHECK( x1.size() == x2.size() );
    };

    for_each_element(hv_with_names, g);
  }


}
