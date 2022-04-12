#if __cplusplus > 201703L
#include "std_e/unit_test/doctest.hpp"

#include "std_e/data_structure/block_range/vblock_iterator.hpp"

using namespace std_e;
using std::vector;

TEST_CASE("vblock_iterator") {
  vector<double> c = {2.,3.,4.,5.,  6.,10.,12.};
  vector<int>   is = {0          ,  4         ,7};

  SUBCASE("const") {
    vblock_iterator<const double,const int> const_it = {c.data(),is.data()};

    CHECK( *const_it == vector{2.,3.,4.,5.} );

    ++const_it;

    CHECK( *const_it == vector{6.,10.,12.} );
  }

  SUBCASE("non-const") {
    vblock_iterator<double,int> it = {c.data(),is.data()};

    vblock_ref<double,int> con0 = *it;
    con0[0] = 42.;
    con0[1] = 43.;
    con0[2] = 44.;
    con0[3] = 45.;

    ++it;

    vblock_ref<double,int> con1 = *it;
    con1[0] = 100.;
    con1[1] = 101.;
    con1[2] = 102.;

    CHECK( c[0] == 42. );
    CHECK( c[1] == 43. );
    CHECK( c[2] == 44. );
    CHECK( c[3] == 45. );
    CHECK( c[4] == 100. );
    CHECK( c[5] == 101. );
    CHECK( c[6] == 102. );
  }

  SUBCASE("assign") {
    vector<double> v0  = {0.,1.,2.};
    vector<int>    is0 = {0       , 3};
    vector<double> v1(3);
    vector<int>    is1(2);

    vblock_iterator<double,int> it0 = {v0.data(),is0.data()};
    vblock_iterator<double,int> it1 = {v1.data(),is1.data()};

    *it1 = *it0;

    CHECK( v1 == vector{0.,1.,2.} );
    CHECK( is1 == vector{0,3} );
  }
}
#endif // C++>17
