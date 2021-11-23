#include "std_e/unit_test/doctest.hpp"
#include "std_e/data_structure/variant_range.hpp"

#include <vector>
#include <variant>
#include <string>

using namespace std;
using namespace std_e;


namespace {


using variant_range_type = variant_range<vector,reference_variant, int,double,string>;

TEST_CASE("variant_range") {
  variant_range_type var_i = vector{10,11,12,13};
  variant_range_type var_b = vector{1.5,1.6,1.7};
  variant_range_type var_s = vector<string>{"Alice","Bob"};

  SUBCASE("size") {
    CHECK( var_i.size() == 4 );
    CHECK( var_b.size() == 3 );
    CHECK( var_s.size() == 2 );
  }

  SUBCASE("operator[]") {
    CHECK( holds_alternative<int>(var_i[0]) );
    CHECK( holds_alternative<double>(var_b[0]) );
    CHECK( holds_alternative<string>(var_s[0]) );

    CHECK( get<int>(var_i[0]) == 10 );
    CHECK( get<int>(var_i[1]) == 11 );
    CHECK( get<int>(var_i[2]) == 12 );
    CHECK( get<int>(var_i[3]) == 13 );
    CHECK( get<double>(var_b[0]) == 1.5 );
    CHECK( get<double>(var_b[1]) == 1.6 );
    CHECK( get<double>(var_b[2]) == 1.7 );
    CHECK( get<string>(var_s[0]) == "Alice" );
    CHECK( get<string>(var_s[1]) == "Bob" );
  }

  SUBCASE("const operator[]") {
    const variant_range_type& var_i_cref = var_i;
    const variant_range_type& var_b_cref = var_b;
    const variant_range_type& var_s_cref = var_s;
    CHECK( get<int>(var_i_cref[0]) == 10 );
    CHECK( get<int>(var_i_cref[1]) == 11 );
    CHECK( get<int>(var_i_cref[2]) == 12 );
    CHECK( get<int>(var_i_cref[3]) == 13 );
    CHECK( get<double>(var_b_cref[0]) == 1.5 );
    CHECK( get<double>(var_b_cref[1]) == 1.6 );
    CHECK( get<double>(var_b_cref[2]) == 1.7 );
    CHECK( get<string>(var_s_cref[0]) == "Alice" );
    CHECK( get<string>(var_s_cref[1]) == "Bob" );
  }

  SUBCASE("operator[] assigns") {
    SUBCASE("direct") {
      var_i[0] = 20;
      var_i[1] = 21;
      var_i[2] = 22;
      var_i[3] = 23;
      var_b[0] = 2.5;
      var_b[1] = 2.6;
      var_b[2] = 2.7;
      var_s[0] = string("Charlie");
      var_s[1] = string("David");
    }

    SUBCASE("by getting the alternative") {
      get<int>(var_i[0]) = 20;
      get<int>(var_i[1]) = 21;
      get<int>(var_i[2]) = 22;
      get<int>(var_i[3]) = 23;
      get<double>(var_b[0]) = 2.5;
      get<double>(var_b[1]) = 2.6;
      get<double>(var_b[2]) = 2.7;
      get<string>(var_s[0]) = string("Charlie");
      get<string>(var_s[1]) = string("David");
    }

    CHECK( get<int>(var_i[0]) == 20 );
    CHECK( get<int>(var_i[1]) == 21 );
    CHECK( get<int>(var_i[2]) == 22 );
    CHECK( get<int>(var_i[3]) == 23 );
    CHECK( get<double>(var_b[0]) == 2.5 );
    CHECK( get<double>(var_b[1]) == 2.6 );
    CHECK( get<double>(var_b[2]) == 2.7 );
    CHECK( get<string>(var_s[0]) == "Charlie" );
    CHECK( get<string>(var_s[1]) == "David" );
  }

  //SUBCASE("operator[] conversions") {
  //  var_i[0] = 20u;
  //  CHECK( get<int>(var_i[0]) == 20 );
  //}
}


} // anonymous
