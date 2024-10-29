#include "std_e/unit_test/doctest.hpp"

#include "std_e/buffer/polymorphic_array.hpp"

TEST_CASE("polymorphic_array") {
  SUBCASE("from owning") {
    std::vector<int> v = {0,1,2};
    std_e::polymorphic_array<int> x(std::move(v));

    CHECK( x.data()[0] == 0 );
    CHECK( x.data()[1] == 1 );
    CHECK( x.data()[2] == 2 );

    CHECK( x.size() == 3 );
  }

  SUBCASE("from non-owning") {
    std::vector<int> v = {0,1,2};
    std_e::span<int> s = std_e::make_span(v.data(),v.size());

    std_e::polymorphic_array<int> x(s); // std::move could also be used: it would do the same
                                        // indeed, only the span will be moved, not its underlying memory
    CHECK( x.data() == v.data() );

    CHECK( x.data()[0] == 0 );
    CHECK( x.data()[1] == 1 );
    CHECK( x.data()[2] == 2 );

    CHECK( x.size() == 3 );
  }

  SUBCASE("detach ownership") {
    std_e::polymorphic_array<int> x(std::vector<int>{0,1,2});
    // Now x contains a vector inside. It is the owner of the vector,
    // and since the vector owns memory, by transitivity, x owns the memory
    CHECK( x.data()[0] == 0 );
    CHECK( x.data()[1] == 1 );
    CHECK( x.data()[2] == 2 );

    auto ptr_to_underlying = x.release();
    // Now x has released its underlying array (here, the vector) ...

    // ... data cannot be accessed through x...
    CHECK( x.is_null() );

    // ... but the memory is still alive and accessible through the released pointer ...
    CHECK( ptr_to_underlying->data()[0] == 0 );
    CHECK( ptr_to_underlying->data()[1] == 1 );
    CHECK( ptr_to_underlying->data()[2] == 2 );

    // ... and memory has to be deleted by deleting the pointer
    delete ptr_to_underlying;
  }

  SUBCASE("detach ownership when no memory") {
    std::vector v = {0,1,2};
    std_e::polymorphic_array<int> x(std_e::make_span(v.data(),v.size()));
    // Now x contains a span inside. It is the owner of the span,
    // and but since the span does not own memory, x does not either:
    // it is still owned by the vector
    CHECK( x.data() == v.data() );

    auto ptr_to_underlying = x.release();
    // Now x has released its underlying array (here, the span) ...

    // ... data cannot be accessed through x...
    CHECK( x.is_null() );

    // ... but the memory is still alive and accessible through the released pointer ...
    CHECK( ptr_to_underlying->data()[0] == 0 );
    CHECK( ptr_to_underlying->data()[1] == 1 );
    CHECK( ptr_to_underlying->data()[2] == 2 );
    // ... this is still the memory of the owning vector ...
    CHECK( ptr_to_underlying->data() == v.data() );

    // ... the span has to be deleted ...
    delete ptr_to_underlying;

    // ... but the memory the span pointed to is still alive,
    // since the span did not own it
    CHECK( v.data()[0] == 0 );
    CHECK( v.data()[1] == 1 );
    CHECK( v.data()[2] == 2 );
  }

  SUBCASE("equality") {
    std_e::polymorphic_array<int> x0;
    std_e::polymorphic_array<int> x1;

    std_e::polymorphic_array<int> x2(std::vector<int>{0,1,2});
    std_e::polymorphic_array<int> x3(std::vector<int>{0,1,2});

    std_e::polymorphic_array<int> x4(std::vector<int>{0,1});

    std::vector<int> v = {0,1,2};
    std_e::polymorphic_array<int> x5(std_e::make_span(v));

    SUBCASE("nullptr cases") {
      CHECK( x0 == x1 );
      CHECK( x0 != x2 );
      CHECK( x2 != x0 );
    }
    SUBCASE("value equality") {
      CHECK( x2 == x3 );
      CHECK( x2 != x4 );
      CHECK( x2 == x5 ); // even if underlying types are differents, the value is the same
    }
    SUBCASE("equality with vector or span") {
      std::vector<int> v = {0,1,2};
      CHECK( x2 == v );
      CHECK( v == x2 );
      CHECK( x2 == std_e::make_span(v) );
      CHECK( std_e::make_span(v) == x2 );

      std::vector<int> w = {1,1,1};
      CHECK( x2 != w );
      CHECK( w != x2 );
      CHECK( x2 != std_e::make_span(w) );
      CHECK( std_e::make_span(w) != x2 );
    }
  }

  SUBCASE("to_string") {
    std::vector<int> v = {0,1,2};
    std_e::polymorphic_array<int> x(std::move(v));
    CHECK( to_string(x) == "{0,1,2}" );
  }
}
