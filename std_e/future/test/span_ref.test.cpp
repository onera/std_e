#include "std_e/unit_test/doctest.hpp"
#include "std_e/future/span_ref.hpp"

#include <vector>

using std::vector;

TEST_CASE("span assignment") {
  SUBCASE("span (view with Regular semantics)") {
    // span is a view, i.e. it does not own its elements
    // but it is still a value type: it has value semantics
    vector<int> v0 = {0,1,2};
    vector<int> v1 = {3,4,5};

    // create s0, make it point to v0
    auto s0 = std_e::make_span(v0);
    CHECK( s0.data() == v0.data() );

    s0 = v1; // s0 points to v1 now
    CHECK( s0.data() == v1.data() );

    // the assignement was shallow,
    // that is
    //    - s0 changed where it was pointing,
    //    - there was no deep assignment, v0 was not changed
    CHECK( v0 == vector{0,1,2} );
  }

  SUBCASE("span_ref (with Reference semantics)") {
    // span is a real reference type
    //   - it does not own its elements
    //   - it is not default constructible
    //   - assignment means that the underlying range is assigned
    vector<int> v0 = {0,1,2};
    vector<int> v1 = {3,4,5};

    //// create s0, make it point to v0
    auto s0 = std_e::make_span_ref(v0);
    CHECK( s0.data() == v0.data() );

    s0 = v1; // s0 still points to v0, but the elements are re-assigned
    CHECK( s0.data() == v0.data() );
    CHECK( v0 == vector{3,4,5} );
  }
  SUBCASE("span_ref swap") {
    vector<int> v0 = {0,1,2};
    vector<int> v1 = {3,4,5};

    auto s0 = std_e::make_span_ref(v0);
    auto s1 = std_e::make_span_ref(v1);

    using std::swap; // should not be used, but bring it here to be sure it is not selected
    swap(s0,s1);

    // swapped values of references...
    CHECK( s0 == vector{3,4,5} );
    CHECK( s1 == vector{0,1,2} );
    // ... and consequently, the values of the ranges underneath:
    CHECK( v0 == vector{3,4,5} );
    CHECK( v1 == vector{0,1,2} );
  }
}
