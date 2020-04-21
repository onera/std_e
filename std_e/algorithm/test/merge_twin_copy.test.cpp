#include "std_e/unit_test/doctest.hpp"

#include "std_e/algorithm/merge_twin_copy.hpp"
#include <vector>
#include <iostream>
using namespace std;

constexpr auto
equals(int i, int j) -> bool {
  return i==j;
}

constexpr auto
add(int i, int j) -> int {
  return i+j;
}

constexpr auto
identity(int i) -> int {
  return i;
}


TEST_CASE("merge_twin_copy simple_cases") {
  vector<int>                v0 = {  1,2,2,3,3,4,5,5,  6};
  vector<int> expected__merged0 = {  1,2+2,3+3,4,5+5,  6};

  vector<int>                v1 = {1,1,2,2,3,3,4,5,5,  6};
  vector<int> expected__merged1 = {1+1,2+2,3+3,4,5+5,  6};

  vector<int>                v2 = {  1,2,2,3,3,4,5,5,6,6};
  vector<int> expected__merged2 = {  1,2+2,3+3,4,5+5,6+6};

  vector<int>                v3 = {1,1,2,2,3,3,4,5,5,6,6};
  vector<int> expected__merged3 = {1+1,2+2,3+3,4,5+5,6+6};

  vector<int> merged0;
  vector<int> merged1;
  vector<int> merged2;
  vector<int> merged3;
  merged0.reserve(v0.size()/2);
  merged1.reserve(v1.size()/2);
  merged2.reserve(v2.size()/2);
  merged3.reserve(v3.size()/2);

  std_e::merge_twin_copy(v0.begin(),v0.end(), back_inserter(merged0), back_inserter(merged0), equals, identity,add);
  std_e::merge_twin_copy(v1.begin(),v1.end(), back_inserter(merged1), back_inserter(merged1), equals, identity,add);
  std_e::merge_twin_copy(v2.begin(),v2.end(), back_inserter(merged2), back_inserter(merged2), equals, identity,add);
  std_e::merge_twin_copy(v3.begin(),v3.end(), back_inserter(merged3), back_inserter(merged3), equals, identity,add);

  CHECK( merged0 == expected__merged0 );
  CHECK( merged1 == expected__merged1 );
  CHECK( merged2 == expected__merged2 );
  CHECK( merged3 == expected__merged3 );
}

TEST_CASE("merge_twin_copy with two outputs") {
  vector<int> v = {1,2,2,3,3,4,5,5,6};
  vector<int> uniques;
  vector<int> twins;

  std_e::merge_twin_copy(v.begin(),v.end(), back_inserter(uniques), back_inserter(twins), equals, identity,add);

  vector<int> expected__uniques = {1,4,6};
  vector<int> expected__twins = {2+2,3+3,5+5};

  CHECK( uniques == expected__uniques );
  CHECK( twins   == expected__twins   );
}


struct S_compress_unique_test {
  int id;
  std::string s;
};
auto
equal_ids(const S_compress_unique_test& x, const S_compress_unique_test& y) -> bool {
  return x.id==y.id;
}
template<class Fwd_it> auto
// requires Fwd_it::value_type = S_compress_unique_test
accumulate_equals_in_vec(Fwd_it first, Fwd_it last, vector<string>& vs) {
  if (first==last) return first;

  string s = first->s;
  Fwd_it next = std::next(first);
  while( next!=last && equal_ids(*first,*next) ) {
    s += next->s;
    ++next;
  }
  vs.push_back(s);
  return next;
}

TEST_CASE("compress_unique") {
  vector<S_compress_unique_test> v = {{1,"a"},{2,"b"},{2,"c"},{2,"d"},{3,"e"},{3,"f"},{4,"g"},{5,"h"},{5,"i"},{6,"j"}};
  vector<string> vs = {};
  auto compress_while = [&vs](auto f, auto l){ return accumulate_equals_in_vec(f,l,vs); };
  std_e::compress_unique(begin(v),end(v),compress_while);

  vector<string> expected_vs = {"a","bcd","ef","g","hi","j"};
  CHECK( vs == expected_vs );
}
