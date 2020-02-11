#include "doctest/doctest.h"
#include "doctest/doctest_utils.hpp"
#include "std_e/data_structure/hierarchical_zip.hpp"

#include <string>

using std::string;

struct S0 {
  int i;
  double d;
};
struct S1 {
  char c;
};
struct S2 {
  S0 s0;
  S1 s1;
  size_t ui;
};
struct S3 {
  S2 s2;
  bool b;
};

struct S0_names {
  string i;
  string d;
};
struct S1_names {
  string c;
};
struct S2_names {
  S0_names s0;
  S1_names s1;
  string ui;
};
struct S3_names {
  S2_names s2;
  string b;
};


TEST_CASE("hzip") {
  S3       s3  = {{{42,3.14},{'a'},10u},true};
  S3_names s3n = {{{"",""  },{""} ,"" },""  };;

  auto s3_with_names = std_e::hzip_view(s3,s3n);

  SUBCASE("access") {
    auto s2_getter = [](auto& s3_like)->auto&{ return s3_like.s2; };
    auto s2_with_names_view = std_e::zip_projection(s3_with_names,s2_getter);

    SUBCASE("getting the right struct") {
      auto& s2_view = std_e::get<S2&>(s2_with_names_view);
      CHECK(s2_view.s0.i==42);
      CHECK(s2_view.s0.d==3.14);
      CHECK(s2_view.s1.c=='a');
      CHECK(s2_view.ui==10u);

      auto& s2n_view = std_e::get<S2_names&>(s2_with_names_view);
      CHECK(s2n_view.s0.i==string(""));
      CHECK(s2n_view.s0.d==string(""));
      CHECK(s2n_view.s1.c==string(""));
      CHECK(s2n_view.ui  ==string(""));
    }
    SUBCASE("modifying a view modifies the object") {
      auto& s2n_view = std_e::get<S2_names&>(s2_with_names_view);
      s2n_view.s0.i="42";
      s2n_view.s0.d="3.14";
      s2n_view.s1.c="a";
      s2n_view.ui  ="10u";

      CHECK(s3n.s2.s0.i==string("42"));
      CHECK(s3n.s2.s0.d==string("3.14"));
      CHECK(s3n.s2.s1.c==string("a"));
      CHECK(s3n.s2.ui  ==string("10u"));
    }
  }
}
