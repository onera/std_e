#include "std_e/unit_test/doctest.hpp"
#include "std_e/future/span.hpp"
#if __cplusplus > 201703L
  #include "std_e/future/concept.hpp"
  #include "std_e/future/ranges/concept.hpp"
#endif

#include <vector>
#include <algorithm>

using std::vector;

#if __cplusplus > 201703L
TEST_CASE("span concepts") {
  static_assert(std_e::ranges::forward_range< std_e::span<int> >);
  static_assert(std_e::ranges::forward_range< std_e::span<int> >);

  static_assert(std_e::permutable< std::ranges::iterator_t<std_e::span<int,3>> >);
  static_assert(std_e::permutable< std::ranges::iterator_t<std_e::span<int,3>> >);
}
#endif

TEST_CASE("span size") {
  constexpr auto sp1size = std_e::span_size<1>();
  constexpr auto sp2size = std_e::span_size<1>();
  CHECK( sp1size == sp2size );
}

TEST_CASE("span constructor") {
  vector<double>       a1  = { 0., 1., 2., 3., 4.};
  const vector<double> ca1 = { 0., 1., 2., 3., 4.};
  vector<double>       a2  = {-1., 6., 3.,-1.,-4.};
  const vector<double> ca2 = {-1., 6., 3.,-1.,-4.};

  double* pta1        = a1.data();
  const double* ptca1 = ca1.data();

  SUBCASE("from ptr static ") {
    auto spc1  = std_e::make_span<3>(pta1);
    auto cspc1 = std_e::make_span<3>(ptca1);

    CHECK( spc1.size()  == 3 );
    CHECK( cspc1.size() == 3 );

    CHECK( spc1[0] == a1[0] );
    CHECK( spc1[1] == a1[1] );
    CHECK( spc1[2] == a1[2] );

    CHECK( cspc1[0] == ca1[0] );
    CHECK( cspc1[1] == ca1[1] );
    CHECK( cspc1[2] == ca1[2] );

    CHECK( spc1.data()  == pta1  );
    CHECK( cspc1.data() == ptca1 );

    CHECK( spc1.begin()  == pta1  );
    CHECK( cspc1.begin() == ptca1 );

    CHECK( spc1.end()  == pta1 +3 );
    CHECK( cspc1.end() == ptca1+3 );
  }

  SUBCASE("from ptr") {
    auto sp1 = std_e::make_span(pta1, 5);
    auto csp1 = std_e::make_span(ptca1, 5);

    CHECK( sp1.size()  == a1.size() );
    CHECK( csp1.size() == ca1.size() );
  }

  SUBCASE("make_subspan_n and make_subspan") {
    std_e::span<double> spo1;
    std_e::span<const double> cspo1;

    SUBCASE("make_subspan_n from ptr") {
      spo1  = std_e::make_subspan_n(pta1, 1, 3);
      cspo1 = std_e::make_subspan_n(ptca1, 1, 3);
    }
    SUBCASE("make_subspan from ptr") {
      spo1  = std_e::make_subspan(pta1, 1, 4);
      cspo1 = std_e::make_subspan(ptca1, 1, 4);
    }
    SUBCASE("make_subspan_n from range") {
      spo1  = std_e::make_subspan_n(a1, 1, 3);
      cspo1 = std_e::make_subspan_n(ca1, 1, 3);
    }
    SUBCASE("make_subspan_n from ptr") {
      spo1  = std_e::make_subspan(a1, 1, 4);
      cspo1 = std_e::make_subspan(ca1, 1, 4);
    }

    CHECK( spo1.size()  == 3 );
    CHECK( cspo1.size() == 3 );

    CHECK( spo1.begin()  == pta1+1  );
    CHECK( cspo1.begin() == ptca1+1 );

    CHECK( spo1.data()  == pta1+1  );
    CHECK( cspo1.data() == ptca1+1 );

    CHECK( spo1.end()  == pta1+4  );
    CHECK( cspo1.end() == ptca1+4 );

    CHECK( spo1[0] == a1[1] );
    CHECK( spo1[1] == a1[2] );
    CHECK( spo1[2] == a1[3] );

    CHECK( cspo1[0] == ca1[1] );
    CHECK( cspo1[1] == ca1[2] );
    CHECK( cspo1[2] == ca1[3] );
  }

  SUBCASE("from container") {
    SUBCASE("explicit conversion") {
      auto sd1  = std_e::make_span(a1);
      auto csd1 = std_e::make_span(ca1);

      CHECK( sd1.size()  == a1.size() );
      CHECK( csd1.size() == ca1.size() );

      CHECK( sd1.data()  == pta1  );
      CHECK( csd1.data() == ptca1 );

      CHECK( sd1.begin()  == pta1  );
      CHECK( csd1.begin() == ptca1 );

      CHECK( sd1[0] == a1[0] );
      CHECK( sd1[1] == a1[1] );
      CHECK( sd1[2] == a1[2] );
      CHECK( sd1[3] == a1[3] );
      CHECK( sd1[4] == a1[4] );

      CHECK( csd1[0] == ca1[0] );
      CHECK( csd1[1] == ca1[1] );
      CHECK( csd1[2] == ca1[2] );
      CHECK( csd1[3] == ca1[3] );
      CHECK( csd1[4] == ca1[4] );
    }
    SUBCASE("implicit conversion") {
      std_e::span<double> sd1  = a1;
      std_e::span<const double> csd1 = ca1;

      CHECK( sd1.size()  == a1.size() );
      CHECK( csd1.size() == ca1.size() );

      CHECK( sd1.data()  == pta1  );
      CHECK( csd1.data() == ptca1 );

      CHECK( sd1.begin()  == pta1  );
      CHECK( csd1.begin() == ptca1 );

      CHECK( sd1[0] == a1[0] );
      CHECK( sd1[1] == a1[1] );
      CHECK( sd1[2] == a1[2] );
      CHECK( sd1[3] == a1[3] );
      CHECK( sd1[4] == a1[4] );

      CHECK( csd1[0] == ca1[0] );
      CHECK( csd1[1] == ca1[1] );
      CHECK( csd1[2] == ca1[2] );
      CHECK( csd1[3] == ca1[3] );
      CHECK( csd1[4] == ca1[4] );
    }
  }

  SUBCASE("comparison") {
    auto st1   = std_e::make_span(a1);
    auto st2   = std_e::make_span(a1);
    auto stc1  = std_e::make_span(ca1);
    auto stc2  = std_e::make_span(ca1);

    auto st3   = std_e::make_span(a2);
    auto stc3  = std_e::make_span(ca2);

    CHECK( st1  == st2 );
    CHECK( stc1 == st2 );
    CHECK( stc1 == stc2 );

    CHECK( st1 != st3   );
    CHECK( stc1 != stc3 );
  }
}
