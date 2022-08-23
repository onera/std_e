#if __cplusplus > 201703L
#include "std_e/unit_test/doctest.hpp"
#include "std_e/serialize/serialize.hpp"


using namespace std_e;
using namespace std;


TEST_CASE("serialize_2") {
  const vector<string> v = {"Alice","Bob","Charlie"};

  auto [szs,serial] = serialize_2(v);
  CHECK( szs == vector<size_t>{5,3,7} );
  CHECK( serial.size() == 5+3+7 );

  auto first = reinterpret_cast<char*>(serial.data());
  auto last  = first + serial.size();
  string serial_as_chars(first,last);
  CHECK( serial_as_chars == "AliceBobCharlie" );
}

TEST_CASE("deserialize_2") {
  const vector<string> v = {"Alice","Bob","Charlie"};

  auto [szs,serial] = serialize_2(v);

  auto w = deserialize_2<vector<string>>(szs,serial);
  CHECK( w == v );
}
#endif // C++20
