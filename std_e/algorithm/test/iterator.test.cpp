#include "std_e/unit_test/doctest.hpp"
#include "std_e/algorithm/iterator.hpp"
#include <vector>

using std::vector;

TEST_CASE("iterators_to_iterators") {
  vector<double> v = {100.,101.,102.,103.};
  vector<double*> v_ptrs = {&v[2],&v[3],&v[0]};

  vector<double> w = v;
  vector<double*> w_ptrs_expected = {&w[2],&w[3],&w[0]};

  SUBCASE("iterator interface") {
    vector<double*> w_ptrs(v_ptrs.size());
    std_e::iterators_to_iterators(begin(v_ptrs),end(v_ptrs),begin(w_ptrs),v.data(),w.data());

    CHECK( w_ptrs == w_ptrs_expected );
  }

  SUBCASE("vector interface") {
    vector<double*> w_ptrs = std_e::iterators_to_iterators(v_ptrs,v.data(),w.data());

    CHECK( w_ptrs == w_ptrs_expected );
  }
}


TEST_CASE("iterators_to_indices") {
  vector<double> v = {100.,101.,102.,103.};
  vector<double*> v_ptrs = {&v[2],&v[3],&v[0]};

  vector<int> v_indices_expected = {2,3,0};

  SUBCASE("vector interface") {
    vector<int> v_indices = std_e::iterators_to_indices(v_ptrs,v.data());

    CHECK( v_indices == v_indices_expected );
  }
}


TEST_CASE("indices_to_iterators") {
  vector<double> v = {100.,101.,102.,103.};
  vector<int> v_indices = {2,3,0};

  vector<double*> v_ptrs_expected = {&v[2],&v[3],&v[0]};

  SUBCASE("vector interface") {
    vector<double*> v_ptrs = std_e::indices_to_iterators(v_indices,v.data());

    CHECK( v_ptrs == v_ptrs_expected );
  }
}


TEST_CASE("equal_iterator_shifts") {
  vector<double> v = {100.,101.,102.,103.};
  vector<double*> v_ptrs = {&v[2],&v[3],&v[0]};

  vector<double> w = {100.,101.,102.,103.};
  vector<double*> w_ptrs_0 = {&w[2],&w[3],&w[0]};
  vector<double*> w_ptrs_1 = {&w[2],&w[0],&w[3]};
  vector<double*> w_ptrs_2 = {&w[2],&w[3]};

  SUBCASE("vector interface") {
    CHECK      ( std_e::equal_iterator_shifts(v_ptrs,w_ptrs_0,v.data(),w.data()) );
    CHECK_FALSE( std_e::equal_iterator_shifts(v_ptrs,w_ptrs_1,v.data(),w.data()) );
    CHECK_FALSE( std_e::equal_iterator_shifts(v_ptrs,w_ptrs_2,v.data(),w.data()) );
  }
}


