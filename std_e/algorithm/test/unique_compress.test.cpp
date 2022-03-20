#include "std_e/unit_test/doctest.hpp"

#include "std_e/algorithm/unique_compress.hpp"
#include "std_e/algorithm/algorithm.hpp"
#include <vector>
#include "std_e/unit_test/id_string.hpp"
#include "std_e/utils/concatenate.hpp"
using namespace std;
using std_e::id_string;
using std_e::append_to_string;
using std_e::equal_ids;


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
  std_e::unique_compress_copy(begin(v),end(v),std::back_inserter(res),compress_while_eq);

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

// concatenation as a reduction {
template<class T> auto
_append_to_vec(T x) -> std::vector<T> {
  return {std::move(x)};
}
template<class T> auto
_append_to_vec(std::vector<T>& v, T x) -> std::vector<T>& {
  v.emplace_back(std::move(x));
  return v;
}
// Intel needs inline
inline auto append_to_vec = [](auto&&... xs){ return _append_to_vec(FWD(xs)...); };

// concatenation as a reduction }
TEST_CASE("unique_compress_stride_copy") {
  const vector<int> v =       {10,11,12,13,14,15,16,17,18,19};
  const vector<int> strides = {1 ,2    ,1 ,1 ,1 ,3       ,1 };

  SUBCASE("+") {
    vector<int> res(strides.size());
    std_e::unique_compress_strides_copy(begin(v),end(v),begin(res),std::plus<>{},strides.data());

    CHECK( res == vector{10, 11+12, 13,14,15, 16+17+18, 19} );
  }
  SUBCASE("concat") {
    vector<vector<int>> res(strides.size());
    std_e::unique_compress_strides_copy(begin(v),end(v),begin(res),append_to_vec,strides.data());

    CHECK( res == vector<vector<int>>{{10}, {11,12}, {13},{14},{15}, {16,17,18}, {19}} );
  }
}
