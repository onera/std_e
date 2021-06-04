#include "std_e/unit_test/doctest.hpp"
#include "std_e/data_structure/heterogenous_vector.hpp"

using namespace std;
using namespace std_e;

// [Sphinx Doc] hvector {
TEST_CASE("hvector") {
  hvector<int,double> hv = { vector{1,2,3,4} , vector{3.14,2.7} };

  SUBCASE("hsize") {
    CHECK( hvector<int,double>::hsize() == 2 );
    CHECK( hv.hsize() == 2 );
  }
  SUBCASE("size") {
    CHECK( hv.size() == 4+2 );
  }

  SUBCASE("assessors") {
    CHECK( std_e::get<int>(hv) == vector{1,2,3,4} );
    CHECK( std_e::get<double>(hv) == vector{3.14,2.7} );
  }
  SUBCASE("assessors from index") {
    CHECK( std_e::get<0>(hv) == vector{1,2,3,4} );
    CHECK( std_e::get<1>(hv) == vector{3.14,2.7} );
  }

  SUBCASE("mutable accessors") {
    std_e::get<int>(hv)[0] = 100;

    CHECK( std_e::get<int>(hv) == vector{100,2,3,4} );
  }

  SUBCASE("push_back") {
    hv.push_back(10000.);

    CHECK( hv.size() == 4+3 );

    CHECK( std_e::get<int>(hv) == vector{1,2,3,4} );
    CHECK( std_e::get<double>(hv) == vector{3.14,2.7,10000.} );
  }
}
// [Sphinx Doc] hvector }

// [Sphinx Doc] hvector for_each algorithms {
TEST_CASE("hvector for_each algorithms") {
  hvector<int,double> hv = { vector{1,2,3,4} , vector{3.14,2.7} };

  SUBCASE("for_each_range") {
    int res = 0;
    auto f = [&res](auto v){ res += 2*v.size(); };
    for_each_range(hv,f);
    CHECK( res == 2*4+2*2 );
  }
  SUBCASE("for_each_element") {
    double sum = 0;
    auto f = [&sum](auto elt){ sum += elt; };
    for_each_element(hv,f);
    CHECK( sum == 1+2+3+4 + 3.14+2.7 );
  }

  SUBCASE("for_each_element_if") {
    std::vector<double> res;
    auto pred = [](auto x){ return x<3; };
    auto f = [&res](auto x){ res.push_back(2.*x); };
    for_each_element_if(hv,pred,f);
    CHECK( res == vector{2., 4., 5.4} );
  }
}
// [Sphinx Doc] hvector for_each algorithms }

// [Sphinx Doc] hvector find algorithms {
TEST_CASE("hvector find algorithms") {
  hvector<int,double> hv = { vector{1,2,3,4} , vector{3.14,2.7} };

  SUBCASE("apply_first") {
    double value;
    auto f = [&value](auto x){ value = 10*x; };

    SUBCASE("case found") {
      auto p = [](auto x){ return double(x)==3.14; };
      bool found = apply_first(hv,p,f);
      CHECK( value == 3.14*10 );
      CHECK( found );
    }
    SUBCASE("case not found") {
      value = 42.5;
      auto p = [](auto){ return false; };
      bool found = apply_first(hv,p,f);
      CHECK( value == 42.5 ); // unchanged
      CHECK( !found );
    }
  }

  SUBCASE("find_apply") {
    double value;
    auto f = [&value](auto x){ value = 10*x; };

    SUBCASE("case found as first type") {
      auto p = [](auto x){ return double(x)==3.; };
      auto [pos_in_tuple,pos_in_vec] = find_apply(hv,p,f);
      CHECK( value == 3*10 );
      CHECK( pos_in_tuple == 0 );
      CHECK( pos_in_vec == 2 );
    }
    SUBCASE("case found as second type") {
      auto p = [](auto x){ return double(x)==3.14; };
      auto [pos_in_tuple,pos_in_vec] = find_apply(hv,p,f);
      CHECK( value == 3.14*10 );
      CHECK( pos_in_tuple == 1 );
      CHECK( pos_in_vec == 0 );
    }
    SUBCASE("case not found") {
      value = 42.5;
      auto p = [](auto){ return false; };
      auto [pos_in_tuple,pos_in_vec] = find_apply(hv,p,f);
      CHECK( value == 42.5 ); // unchanged
      CHECK( pos_in_tuple == 2 );
      CHECK( pos_in_vec == 2 );
    }
  }

  SUBCASE("find_position") {
    auto p = [](auto x){ return double(x)==3.14; };
    auto [pos_in_tuple,pos_in_vec] = find_position(hv,p);
    CHECK( pos_in_tuple == 1 );
    CHECK( pos_in_vec == 0 );
  }
}
// [Sphinx Doc] hvector find algorithms }


#if __cplusplus > 201703L
TEST_CASE("hvector deduction guidelines") {
  SUBCASE("rvalue") {
    // [Sphinx Doc] hvector deduction {
    hvector hv = { vector{1,2,3,4} , vector{3.14,2.7} };
    static_assert( std::is_same_v< decltype(hv) , hvector<int,double> > );
    CHECK( std_e::get<int>(hv) == vector{1,2,3,4} );
    CHECK( std_e::get<double>(hv) == vector{3.14,2.7} );
    // [Sphinx Doc] hvector deduction }
  }
  SUBCASE("ref") {
    vector vi = {1,2,3,4};
    vector vd = {3.14,2.7};
    hvector hv = {vi,vd};
    static_assert( std::is_same_v< decltype(hv) , hvector<int,double> > );
    CHECK( std_e::get<int>(hv) == vector{1,2,3,4} );
    CHECK( std_e::get<double>(hv) == vector{3.14,2.7} );
  }
  SUBCASE("const ref") {
    const vector vi = {1,2,3,4};
    const vector vd = {3.14,2.7};
    hvector hv = {vi,vd};
    static_assert( std::is_same_v< decltype(hv) , hvector<int,double> > );
    CHECK( std_e::get<int>(hv) == vector{1,2,3,4} );
    CHECK( std_e::get<double>(hv) == vector{3.14,2.7} );
  }
}
#endif // C++20
