#include "std_e/unit_test/doctest.hpp"

#include "std_e/algorithm/merge_twin_copy.hpp"
#include "std_e/algorithm/algorithm.hpp"
#include <vector>
#include "std_e/unit_test/id_string.hpp"
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


template<class Fwd_it, class S, class T, class Bin_op, class Bin_pred> auto
accumulate_equals_in_vec(Fwd_it first, S last, T init, Bin_op op, Bin_pred p, vector<string>& vs) -> Fwd_it {
  auto [next,res] = std_e::accumulate_while_adjacent(first,last,init,op,p);
  vs.push_back(res);
  return next;
}

TEST_CASE("unique_compress") {
  vector<id_string> v = {{0,"0"},{1,"a"},{2,"b"},{2,"c"},{2,"d"},{3,"e"},{3,"f"},{4,"g"},{5,"h"},{5,"i"},{6,"j"}};
  vector<string> vs = {};
  auto compress_while_eq = [&vs](auto f, auto l){ return accumulate_equals_in_vec(f,l,string{},append_to_string,equal_ids,vs); };
  auto pos = std_e::unique_compress(begin(v),end(v),compress_while_eq);

  vector<string> expected_vs = {"0","a","bcd","ef","g","hi","j"};
  CHECK( vs == expected_vs );

  CHECK( (pos-begin(v)) == expected_vs.size() );
  CHECK( v[0] == id_string{0,"0"} );
  CHECK( v[1] == id_string{1,"a"} );
  CHECK( v[2] == id_string{2,"b"} );
  CHECK( v[3] == id_string{3,"e"} );
  CHECK( v[4] == id_string{4,"g"} );
  CHECK( v[5] == id_string{5,"h"} );
  CHECK( v[6] == id_string{6,"j"} );
}

TEST_CASE("unique_compress_copy") {
  const vector<id_string> v = {{0,"0"},{1,"a"},{2,"b"},{2,"c"},{2,"d"},{3,"e"},{3,"f"},{4,"g"},{5,"h"},{5,"i"},{6,"j"}};
  vector<string> vs = {};
  auto compress_while_eq = [&vs](auto f, auto l){ return accumulate_equals_in_vec(f,l,string{},append_to_string,equal_ids,vs); };

  vector<id_string> res;
  auto pos = std_e::unique_compress_copy(begin(v),end(v),std::back_inserter(res),compress_while_eq);

  vector<string> expected_vs = {"0","a","bcd","ef","g","hi","j"};
  CHECK( vs == expected_vs );

  CHECK( res.size() == expected_vs.size() );
  CHECK( res[0] == id_string{0,"0"} );
  CHECK( res[1] == id_string{1,"a"} );
  CHECK( res[2] == id_string{2,"b"} );
  CHECK( res[3] == id_string{3,"e"} );
  CHECK( res[4] == id_string{4,"g"} );
  CHECK( res[5] == id_string{5,"h"} );
  CHECK( res[6] == id_string{6,"j"} );
}

TEST_CASE("unique_compress_copy_with_index_position") {
  const vector<id_string> v = {{40,"0"},{41,"a"},{42,"b"},{42,"c"},{42,"d"},{43,"e"},{43,"f"},{44,"g"},{45,"h"},{45,"i"},{46,"j"}};
  vector<string> vs = {};
  auto compress_while_eq = [&vs](auto f, auto l){ return accumulate_equals_in_vec(f,l,string{},append_to_string,equal_ids,vs); };

  vector<id_string> compress_res;
  vector<int> positions_old_to_new;
  std_e::unique_compress_copy_with_index_position(
    begin(v),end(v),
    std::back_inserter(compress_res),
    0,std::back_inserter(positions_old_to_new),
    compress_while_eq
  );

  vector<int> expected_positions_old_to_new = {0,1,2,2,2,3,3,4,5,5,6};
  CHECK( positions_old_to_new == expected_positions_old_to_new );
}
