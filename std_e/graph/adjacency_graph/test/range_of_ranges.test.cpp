#include "std_e/unit_test/doctest.hpp"

#include "std_e/graph/adjacency_graph/range_of_ranges.hpp"

#include <vector>
#include <deque>
#include <string>
using namespace std;

TEST_CASE("range_of_ranges") {
  using ror_0 = std_e::range_of_ranges<int,vector>;
  using ror_1 = std_e::range_of_ranges<long,vector,deque>;

  // a range_of_ranges is just a range of ranges
  ror_0 x = { {4,3,2},{0,1} };
  CHECK( x[0] == vector{4,3,2} );
  CHECK( x[1] == vector{0,1} );

  // retrive value_type
  static_assert(is_same_v< std_e::value_type<ror_0> , int  >); // default to int
  static_assert(is_same_v< std_e::value_type<ror_1> , long >);

  // retrieve inner_range type
  static_assert(is_same_v< std_e::inner_range_type<ror_0> , vector<int> >); // default
  static_assert(is_same_v< std_e::inner_range_type<ror_1> , deque<long> >);

  // create range from outer_range template type
  static_assert(is_same_v< std_e::range_from_outer<ror_0,string> , vector<string> >);
  static_assert(is_same_v< std_e::range_from_outer<ror_1,string> , vector<string> >);

  // create a range_of_ranges with another value_type
  using str_ror = std_e::range_of_ranges_from<ror_1,string>;
  str_ror y = { {"A","B"}, {"C"} };
  CHECK( y[0] == deque<string>{"A","B"} );
  CHECK( y[1] == deque<string>{"C"} );
  static_assert(is_same_v< str_ror , vector<deque<string>> >);
}
