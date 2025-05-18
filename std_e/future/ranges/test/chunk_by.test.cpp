#if __cplusplus > 201703L
#include "std_e/unit_test/doctest.hpp"

#include "std_e/future/ranges/chunk_by.hpp"
#include <vector>
#include <functional>

using std::vector;
using std_e::span;


TEST_CASE("repeat") {
  vector v = {'A','A','C','A','B','B','B','C'};

  std_e::vblock_range<vector<char>,vector<int>> expected_chunked_result = {
    {'A','A'},{'C'},{'A'},{'B','B','B'},{'C'}
  };
  SUBCASE("function syntax") {
    std_e::vblock_range<span<char>,vector<int>> w = std_e::chunk_by(v,std::equal_to<>{});

    CHECK( w == expected_chunked_result );
  }
  SUBCASE("pipe syntax") {
    auto w = v | std_e::chunk_by(std::equal_to<>{});

    CHECK( w == expected_chunked_result );
  }
}
#endif // C++>17
