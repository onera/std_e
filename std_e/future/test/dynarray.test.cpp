#if __cplusplus > 201703L
#include "std_e/unit_test/doctest.hpp"
#include "std_e/future/dynarray.hpp"

#include <vector>
#include <algorithm>

using std_e::dynarray;

TEST_CASE("dynarray") {
  SUBCASE("default ctor") {
    dynarray<int> x;
    CHECK( x.size() == 0 );
    CHECK( x.data() == nullptr );
  }
  SUBCASE("ctor from size + iterators and pointers") {
    dynarray<int> x(10); // Note: contrary to std::vector, dynarray does not initialize its content
    CHECK( x.size() == 10 );
    CHECK( x.data() != nullptr );
    CHECK( x.data() == x.begin() );
    CHECK( x.end() == x.begin()+x.size() );
    CHECK( x.begin() == begin(x) );
    CHECK( x.end()   == end(x) );
  }

  SUBCASE("[first,last) ctor + read access") {
    std::vector<int> buffer = {5,3,6,1};
    dynarray<int> x(buffer.begin(), buffer.end());

    CHECK( x.size() == 4 );
    CHECK( x[0] == 5 );
    CHECK( x[1] == 3 );
    CHECK( x[2] == 6 );
    CHECK( x[3] == 1 );

    CHECK( x.front() == 5 );
    CHECK( x.back()  == 1 );

    // returns references
    CHECK( &x[0] == x.data()+0 );
    CHECK( &x[1] == x.data()+1 );
    CHECK( &x[2] == x.data()+2 );
    CHECK( &x[3] == x.data()+3 );

    CHECK( &x.front() == x.data()+0 );
    CHECK( &x.back()  == x.data()+3 );
  }

  SUBCASE("initialisation list ctor") {
    dynarray<int> x = {5,3,6,1};
    CHECK( x.size() == 4 );
    CHECK( x[0] == 5 );
    CHECK( x[1] == 3 );
    CHECK( x[2] == 6 );
    CHECK( x[3] == 1 );
  }

  SUBCASE("write access") {
    dynarray<int> x = {5,3,6,1};
    x[1] = 333;
    CHECK( x[0] == 5   );
    CHECK( x[1] == 333 );
    CHECK( x[2] == 6   );
    CHECK( x[3] == 1   );

    x.front() = 555;
    x.back()  = 111;
    CHECK( x[0] == 555 );
    CHECK( x[1] == 333 );
    CHECK( x[2] == 6   );
    CHECK( x[3] == 111 );
  }

  SUBCASE("to_string") {
    dynarray<int> x = {5,3,6,1};
    CHECK( to_string(x) == "{5,3,6,1}" );  
  }

  SUBCASE("comparisons") {
    dynarray<int> x = {5,3,6,1};
    dynarray<int> y = {5,3,6,1};
    dynarray<int> z = {5,3,7,1};
    dynarray<int> t = {5,3,6};
    CHECK( x == y );  
    CHECK( x != z );  
    CHECK( x <  z );  
    CHECK( x >  t );  
  }

  SUBCASE("move ctor") {
    dynarray<int> x = {5,3,6,1};
    dynarray<int> y(std::move(x));

    CHECK( x.data() == nullptr );
    CHECK( x.size() == 0 );

    CHECK( y.size() == 4 );
    CHECK( y[0] == 5 );
    CHECK( y[1] == 3 );
    CHECK( y[2] == 6 );
    CHECK( y[3] == 1 );
  }

  SUBCASE("move assign") {
    dynarray<int> x = {5,3,6,1};
    dynarray<int> y = {10, 11, 12};

    y = std::move(x);

    CHECK( y.size() == 4 );
    CHECK( y[0] == 5 );
    CHECK( y[1] == 3 );
    CHECK( y[2] == 6 );
    CHECK( y[3] == 1 );
  }

  SUBCASE("copy ctor") {
    dynarray<int> x = {5,3,6,1};
    dynarray<int> y(x);

    CHECK( y.size() == 4 );
    CHECK( y[0] == 5 );
    CHECK( y[1] == 3 );
    CHECK( y[2] == 6 );
    CHECK( y[3] == 1 );

    y[0] = 100;
    CHECK( y[0] == 100 );
    CHECK( x[0] == 5 ); // the copy is a deep copy
  }
  SUBCASE("copy assign") {
    dynarray<int> x = {5,3,6,1};
    dynarray<int> y = {10, 11, 12};

    y = x;

    CHECK( y.size() == 4 );
    CHECK( y[0] == 5 );
    CHECK( y[1] == 3 );
    CHECK( y[2] == 6 );
    CHECK( y[3] == 1 );

    y[0] = 100;
    CHECK( y[0] == 100 );
    CHECK( x[0] == 5 ); // the copy is a deep copy
  }
}
#endif
