#if __cplusplus > 201703L
#include "std_e/unit_test/doctest.hpp"

#include "std_e/data_structure/block_range/ivblock_ref.hpp"

using namespace std_e;
using std::vector;

TEST_CASE("ivblock_ref") {
  vector<int> v = {3, 1,2,3};
  ivblock_ref<const int> cref(v.data());
  ivblock_ref<      int>  ref(v.data());

  SUBCASE("basic_tests") {
    CHECK( ref.size()  == 3 );
  }

  SUBCASE("equality") {
    vector<int> v0 = {3, 1,2,3};
    ivblock_ref<int> ref0(v0.data());

    vector<int> v1 = {3, 5,2,3};
    ivblock_ref<int> ref1(v1.data());

    vector<int> v2 = {2, 1,2};
    ivblock_ref<int> ref2(v2.data());

    vector<int> v3 = {2, 5,2};
    ivblock_ref<int> ref3(v3.data());

    vector<int> v4 = {4, 1,2,3,4};
    ivblock_ref<int> ref4(v4.data());

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
    CHECK( ref.begin() == v.data()+1  );
    CHECK( ref.end()   == v.data()+1+3);
  }

  SUBCASE("assign same size") {
    vector<int> v0 = {3, 5,6,7};

    SUBCASE("from const") {
      ivblock_ref<const int> cref0(v0.data());
      ref = cref0;

      CHECK( v == vector{3, 5,6,7} );
    }
    SUBCASE("from non-const") {
      ivblock_ref<int> ref0(v0.data());
      ref = ref0;

      CHECK( v == vector{3, 5,6,7} );
    }
  }

  SUBCASE("assign different size") {
    SUBCASE("smaller") {
      vector<int> v0 = {2, 5,6};
      ivblock_ref<int> ref0(v0.data());
      ref = ref0;

      CHECK( v == vector{2, 5,6,3} ); // Note: the last element was not override
    }
    SUBCASE("longer") {
      vector<int> v0 = {4, 5,6,7,8};
      ivblock_ref<int> ref0(v0.data());

      v.resize(5); // need to have a size large enought at the destination

      // Warning: `ref` is no longer valid since `v.data()` was invalidated by `v.resize()`
      // We need to re-create a reference
      ivblock_ref<int> ref1(v.data());

      // assignment
      ref1 = ref0;

      CHECK( v == vector{4, 5,6,7,8} );
    }
  }
}
#endif // C++>17
