#include "std_e/unit_test/doctest.hpp"

#include "std_e/field/md_field.hpp"

using namespace std_e;

TEST_CASE("concepts") {
  static_assert( Scalar_field< scalar_field >);
  static_assert(!Scalar_field< vector_field<3> >);

  static_assert( Vector_field< vector_field<3> >);
  static_assert(!Vector_field< tensor_field<3,2> >);
  static_assert( Vector_field_of_dim< vector_field<3>, 3 >);
  static_assert(!Vector_field_of_dim< vector_field<3>, 2 >);

  static_assert(!Tensor_field< vector_field<3> >);
  static_assert( Tensor_field< tensor_field<3,2> >);
  static_assert( Tensor_field_of_dims< tensor_field<3,2>, 3,2 >);
  static_assert(!Tensor_field_of_dims< tensor_field<3,2>, 2,3 >);
}
TEST_CASE("field") {
  scalar_field x = {0.,1.,2.,3.,4.};
  
  auto sub_x = x.sub(3);
  CHECK( sub_x.size() == 3 );
  CHECK( sub_x(0) == 0. );
  CHECK( sub_x(1) == 1. );
  CHECK( sub_x(2) == 2. );
}

TEST_CASE("md_field") {
  using md_field_t = md_field<double, mallocator, 3,2>;
  static_assert(std::is_same_v<md_field_t::value_type, double>);
  static_assert(md_field_t::rank == 2);
  static_assert(md_field_t::dims == std::array<int,2>{3,2});
  static_assert(md_field_t::dim_tot == 6);

  static_assert(Field<md_field_t>);

  SUBCASE("ctor uniform") {
    vector_field<3> x(2, 7.);
    CHECK( x.underlying() == std::array<std_e::dynarray<double>,3>{std_e::dynarray{7.,7.}, std_e::dynarray{7.,7.}, std_e::dynarray{7.,7.}} );
  }
  SUBCASE("field() const") {
    vector_field<3> x(2, 7.);

    CHECK( x.field(0) == std::vector{7.,7.} );
    CHECK( x.field(1) == std::vector{7.,7.} );
    CHECK( x.field(2) == std::vector{7.,7.} );
  }

  SUBCASE("field()") {
    vector_field<3> x(2);

    x.field(0) = std::vector{ 0., 1.};
    x.field(1) = std::vector{10.,11.};
    x.field(2) = std::vector{20.,21.};

    CHECK( x.underlying() == std::array<std_e::dynarray<double>,3>{std_e::dynarray{0.,1.}, std_e::dynarray{10.,11.}, std_e::dynarray{20.,21.}} );

  }

  SUBCASE("element access") {
    vector_field<3> x(2);

    x(0, 0) =  0.; x(1, 0) =  1.; // field 0
    x(0, 1) = 10.; x(1, 1) = 11.; // field 1
    x(0, 2) = 20.; x(1, 2) = 21.; // field 2

    CHECK( x.underlying() == std::array<std_e::dynarray<double>,3>{std_e::dynarray{0.,1.}, std_e::dynarray{10.,11.}, std_e::dynarray{20.,21.}} );
  }
}

TEST_CASE("md_field_view") {
  vector_field<3> x(2, 7.);

  vector_field_view<3> x_view(x);

  CHECK( x_view.field(0) == std::vector{7.,7.} );
  CHECK( x_view.field(1) == std::vector{7.,7.} );
  CHECK( x_view.field(2) == std::vector{7.,7.} );

  x_view(1,2) = 10.;
  x(1,0) = 14.;


  CHECK( x_view.field(0) == std::vector{7.,14.} );
  CHECK( x_view.field(1) == std::vector{7., 7.} );
  CHECK( x_view.field(2) == std::vector{7.,10.} );

  CHECK( x_view.field(0) == std::vector{7.,14.} );
  CHECK( x_view.field(1) == std::vector{7., 7.} );
  CHECK( x_view.field(2) == std::vector{7.,10.} );
}

TEST_CASE("v_stack") {
  SUBCASE("vector") {
    scalar_field    x(1);
    vector_field<3> y(1);
    scalar_field    z(1);

    x(0  ) = 1.;
    y(0,0) = 2.;
    y(0,1) = 3.;
    y(0,2) = 4.;
    z(0  ) = 5.;

    auto res = v_stack(x,y,z);

    CHECK( res(0,0) == 1. );
    CHECK( res(0,1) == 2. );
    CHECK( res(0,2) == 3. );
    CHECK( res(0,3) == 4. );
    CHECK( res(0,4) == 5. );
  }
  SUBCASE("tensor") {
    vector_field<3>   x(1);
    tensor_field<3,3> y(1);
    vector_field<3>   z(1);

    x(0,0  ) =  0.; x(0,1  ) =  1.; x(0,2  ) =  2.;
    y(0,0,0) =  3.; y(0,0,1) =  4.; y(0,0,2) =  5.;
    y(0,1,0) =  6.; y(0,1,1) =  7.; y(0,1,2) =  8.;
    y(0,2,0) =  9.; y(0,2,1) = 10.; y(0,2,2) = 11.;
    z(0,0  ) = 12.; z(0,1  ) = 13.; z(0,2  ) = 14.;

    auto res = v_stack(x,y,z);

    CHECK( res(0,0,0) ==  0. ); CHECK( res(0,0,1) ==  1. ); CHECK( res(0,0,2) ==  2. );
    CHECK( res(0,1,0) ==  3. ); CHECK( res(0,1,1) ==  4. ); CHECK( res(0,1,2) ==  5. );
    CHECK( res(0,2,0) ==  6. ); CHECK( res(0,2,1) ==  7. ); CHECK( res(0,2,2) ==  8. );
    CHECK( res(0,3,0) ==  9. ); CHECK( res(0,3,1) == 10. ); CHECK( res(0,3,2) == 11. );
    CHECK( res(0,4,0) == 12. ); CHECK( res(0,4,1) == 13. ); CHECK( res(0,4,2) == 14. );
  }
}

TEST_CASE("row") {
  SUBCASE("row of vector") {
    vector_field<3> x(1);

    x(0,0) =  1.;
    x(0,1) =  2.;
    x(0,2) =  3.;

    auto res = row(x,1);

    CHECK( res(0) ==  2. );
  }
  SUBCASE("row of tensor") {
    tensor_field<3,2> x(1);

    x(0,0,0) =  1.; x(0,0,1) =  2.;
    x(0,1,0) =  3.; x(0,1,1) =  4.;
    x(0,2,0) =  5.; x(0,2,1) =  6.;

    auto res = row(x,1);

    CHECK( res(0,0) ==  3. ); CHECK( res(0,1) ==  4. );
  }
}

TEST_CASE("md_field_uniform") {
  SUBCASE("vector") {
    vector_field_uniform<3> x = {4.,5.,6.};

    CHECK( x(0) == 4. );
    CHECK( x(1) == 5. );
    CHECK( x(2) == 6. );
  }
  SUBCASE("tensor") {
    tensor_field_uniform<3,2> x = {
      {1.,2.},
      {3.,4.},
      {5.,6.}
    };
    CHECK( x(0,0) ==  1.); CHECK( x(0,1) ==  2.);
    CHECK( x(1,0) ==  3.); CHECK( x(1,1) ==  4.);
    CHECK( x(2,0) ==  5.); CHECK( x(2,1) ==  6.);
  }
}
