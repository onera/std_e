#include "doctest/doctest.h"
#include "std_e/utils/doctest_utils.hpp"
#include "std_e/data_structure/hierarchical_zip.hpp"
#include "std_e/data_structure/heterogenous_vector.hpp"

#include <string>

using std::string;
using std::vector;
using std_e::hvector;

struct HS0 {
  int i;
};
struct HS1 {
  double d;
};
auto to_string(const HS0& x) { return std::to_string(    x.i ); }
auto to_string(const HS1& x) { return std::to_string(int(x.d)); }
auto compare_to(const HS0& x, int i) { return     x.i  == i; }
auto compare_to(const HS1& x, int i) { return int(x.d) == i; }

struct HS0_names {
  string i;
};
struct HS1_names {
  string d;
};
auto set_str_val(HS0_names& x, const string& s) { x.i = s; }
auto set_str_val(HS1_names& x, const string& s) { x.d = s; }

auto to_string(const HS0_names& x) { return x.i; }
auto to_string(const HS1_names& x) { return x.d; }

// struct fundamental_hierarchy {
//   std_e::hvector<HS0> level1;
// };

// struct optional_hierarchy_1 {
//   std_e::hvector<HS0_names> level1;
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
  hvector<HS0,HS1>             hv  = { vector{HS0{10},HS0{11},HS0{12}} , vector{HS1{13.},HS1{14.}} };
  hvector<HS0_names,HS1_names> hvn = { vector<HS0_names>(3)            , vector<HS1_names>(2)     };

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
      // std::cout << __PRETTY_FUNCTION__ << std::endl;
      // std::cout << to_string(x1) << " <-> " << to_string(x2) << std::endl;
      // std::cout << " x1.i " << x1.i << " <-> " << " x2.i " << x2.i << std::endl;
      // assert( x1.size() == x2.size() );
      // CHECK( x1.size() == x2.size() );
    };

    for_each_element(hv_with_names, g);
  }


}
