#include "std_e/unit_test/doctest.hpp"
#include "std_e/algorithm/algorithm.hpp"
#include "std_e/unit_test/id_string.hpp"
#include <vector>

using namespace std;
using namespace std_e;

TEST_CASE("equal_n") {
  vector<int> v0 = {2,3,4,5};

  vector<int> v1 = {2,3,4,5};
  CHECK( equal_n(begin(v0),begin(v1),4) );

  vector<int> v2 = {42,3,4,5};
  CHECK_FALSE( equal_n(begin(v0),begin(v2),4) );

  vector<int> v3 = {2,3,4,42};
  CHECK_FALSE( equal_n(begin(v0),begin(v3),4) );

  vector<int> v4 = {2,3,4};
  CHECK( equal_n(begin(v0),begin(v4),3) );
}

TEST_CASE("equal_n with predicate") {
  auto pred = [](int i, int j){ return i%2 == j%5; };

  vector<int> v0 = {2,3,4,5};

  vector<int> v1 = {0,1,0,1};
  CHECK( equal_n(begin(v0),begin(v1),4,pred) );

  vector<int> v2 = {1,1,0,1};
  CHECK_FALSE( equal_n(begin(v0),begin(v2),4,pred) );

  vector<int> v3 = {0,1,0};
  CHECK( equal_n(begin(v0),begin(v3),3,pred) );
}
TEST_CASE("accumulate_while_adjacent") {
  vector<id_string> v = {{1,"a"},{1,"b"},{1,"c"},{2,"d"},{3,"e"},{3,"f"}};
  auto append_to_string = [](string& s, const id_string& x){ s += x.s; return s; };
  auto [pos0,res0] = accumulate_while_adjacent (begin(v),end(v),string{},append_to_string,equal_ids);
  CHECK( pos0 == begin(v)+3 );
  CHECK( res0 == "abc" );
  auto [pos1,res1] = accumulate_while_adjacent (pos0,end(v),string("Hello "),append_to_string,equal_ids);
  CHECK( pos1 == begin(v)+4 );
  CHECK( res1 == "Hello d" );
  auto [pos2,res2] = accumulate_while_adjacent (pos1,end(v),string{},append_to_string,equal_ids);
  CHECK( pos2 == end(v) );
  CHECK( res2 == "ef" );
}


TEST_CASE("offset") {
  vector<int>          v = { 10, 100, 1000 };
  vector<int> expected_v = { 12, 102, 1002 };

  int val = 2;
  std_e::offset(v,val);

  CHECK( v == expected_v );
}
