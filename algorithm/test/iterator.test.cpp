#include "doctest/doctest.h"
#include "std_e/algorithm/iterator.hpp"
#include <vector>

using std::vector;

TEST_CASE("transfer_iterator_shifts") {
  vector<double> v = {100.,101.,102.,103.};
  vector<double*> v_ptrs = {&v[2],&v[3],&v[0]};

  vector<double> w = v;
  vector<double*> w_ptrs_expected = {&w[2],&w[3],&w[0]};

  vector<double*> w_ptrs(v_ptrs.size());
  std_e::transfer_iterator_shifts(begin(v_ptrs),end(v_ptrs),begin(w_ptrs),v.data(),w.data());

  CHECK( w_ptrs == w_ptrs_expected );
}
