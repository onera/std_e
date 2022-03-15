#include "std_e/unit_test/doctest.hpp"

#include "std_e/data_structure/block_range/vblock_ref.hpp"

using namespace std_e;
using std::vector;

TEST_CASE("vblock_ref") {
  vector<double> v = {1,2,3};
  vector<int>   is = {0    ,3};
  vblock_ref<const double,const int> cref(v.data(),is.data());
  vblock_ref<      double,      int>  ref(v.data(),is.data());

  SUBCASE("basic_tests") {
    CHECK( ref.size()  == 3 );
  }

  SUBCASE("equality") {
    vector<double> v0 = {1,2,3};
    vector<int>   is0 = {0    ,3};
    vblock_ref<double,int> ref0(v0.data(),is0.data());

    vector<double> v1 = {5,2,3};
    vector<int>   is1 = {0    ,3};
    vblock_ref<double,int> ref1(v1.data(),is1.data());

    vector<double> v2 = {1,2};
    vector<int>   is2 = {0    ,2};
    vblock_ref<double,int> ref2(v2.data(),is2.data());

    vector<double> v3 = {5,2};
    vector<int>   is3 = {0    ,2};
    vblock_ref<double,int> ref3(v3.data(),is3.data());

    vector<double> v4 = {1,2,3,4};
    vector<int>   is4 = {0      ,4};
    vblock_ref<double,int> ref4(v4.data(),is4.data());

    CHECK( ref == ref0 );
    CHECK( ref != ref1 );
    CHECK( ref != ref2 );
    CHECK( ref != ref3 );
    CHECK( ref != ref4 );

    CHECK( !(ref < ref0) );
    CHECK(   ref < ref1  );
    CHECK( !(ref < ref2) );
    CHECK( !(ref < ref3) );
    CHECK(   ref < ref4  );
  }


  SUBCASE("read") {
    CHECK( cref[0] == 1 );
    CHECK( cref[1] == 2 );
    CHECK( cref[2] == 3 );
  }

  SUBCASE("write") {
    ref[0] = 10;
    ref[1] = 11;
    ref[2] = 12;

    CHECK( cref[0] == 10 );
    CHECK( cref[1] == 11 );
    CHECK( cref[2] == 12 );
  }

  SUBCASE("begin and end") {
    CHECK( ref.begin() == v.data()  );
    CHECK( ref.end()   == v.data()+3);
  }

  SUBCASE("assign same size") {
    vector<double> v0 = {5.,6.,7.};
    vector<int>   is0 = {0       ,3};

    SUBCASE("from const") {
      vblock_ref<const double,const int> cref0(v0.data(),is0.data());
      ref = cref0;

      CHECK( v == vector{5.,6.,7.} );
      CHECK( is == vector{0,3} );
    }
    SUBCASE("from non-const") {
      vblock_ref<double,int> ref0(v0.data(),is0.data());
      ref = ref0;

      CHECK( v == vector{5.,6.,7.} );
      CHECK( is == vector{0,3} );
    }
  }

  SUBCASE("assign different size") {
    SUBCASE("smaller") {
      vector<double> v0 = {5.,6.};
      vector<int>   is0 = {0    ,2};
      vblock_ref<double,int> ref0(v0.data(),is0.data());
      ref = ref0;

      CHECK( v == vector{5.,6.,3.} ); // Note: the last element was not override
      CHECK( is == vector{0,2} );
    }
    SUBCASE("longer") {
      vector<double> v0 = {5.,6.,7.,8.};
      vector<int>   is0 = {0          ,4};
      vblock_ref<double,int> ref0(v0.data(),is0.data());

      v.resize(4); // need to have a size large enought at the destination

      // Warning: `ref` is no longer valid since `v.data()` was invalidated by `v.resize()`
      // We need to re-create a reference
      vblock_ref<double,int> ref1(v.data(),is.data());

      // assignment
      ref1 = ref0;

      CHECK( v == vector{5.,6.,7.,8.} );
      CHECK( is == vector{0,4} );
    }
  }
}
