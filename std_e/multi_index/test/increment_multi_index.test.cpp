#include "std_e/unit_test/doctest.hpp"
#include "std_e/multi_index/multi_index_range.hpp"
#include "std_e/log.hpp"


using namespace std_e;


TEST_CASE("increment_multi_index fortran order") {
  using MI = multi_index<int,2>;
  MI dims = {3,2};

  const MI ci0 = {0,0};    MI i0 = {0,0};
  const MI ci1 = {1,0};    MI i1 = {1,0};
  const MI ci2 = {2,0};    MI i2 = {2,0};
  const MI ci3 = {0,1};    MI i3 = {0,1};
  const MI ci4 = {1,1};    MI i4 = {1,1};
  const MI ci5 = {2,1};    MI i5 = {2,1};

  int d0 = increment_multi_index_fortran_order(dims,i0);
  int d1 = increment_multi_index_fortran_order(dims,i1);
  int d2 = increment_multi_index_fortran_order(dims,i2);
  int d3 = increment_multi_index_fortran_order(dims,i3);
  int d4 = increment_multi_index_fortran_order(dims,i4);
  int d5 = increment_multi_index_fortran_order(dims,i5);


  CHECK( i0 == ci1 );
  CHECK( i1 == ci2 );
  CHECK( i2 == ci3 );
  CHECK( i3 == ci4 );
  CHECK( i4 == ci5 );
  CHECK( i5 == ci0 ); // not incremented (impossible)

  CHECK( d0 == 0 );
  CHECK( d1 == 0 );
  CHECK( d2 == 1 );
  CHECK( d3 == 0 );
  CHECK( d4 == 0 );
  CHECK( d5 == 2 );
}


TEST_CASE("increment_multi_index general order") {
  using MI = multi_index<int,2>;
  MI dims = {3,2};
  MI order = {1,0}; // inverse order ...
  // .. means C-order (row-major)
  const MI ci0 = {0,0};    MI i0 = {0,0};
  const MI ci1 = {0,1};    MI i1 = {0,1};
  const MI ci2 = {1,0};    MI i2 = {1,0};
  const MI ci3 = {1,1};    MI i3 = {1,1};
  const MI ci4 = {2,0};    MI i4 = {2,0};
  const MI ci5 = {2,1};    MI i5 = {2,1};

  int d0 = increment_multi_index(dims,i0,order);
  int d1 = increment_multi_index(dims,i1,order);
  int d2 = increment_multi_index(dims,i2,order);
  int d3 = increment_multi_index(dims,i3,order);
  int d4 = increment_multi_index(dims,i4,order);
  int d5 = increment_multi_index(dims,i5,order);

  CHECK( i0 == ci1 );
  CHECK( i1 == ci2 );
  CHECK( i2 == ci3 );
  CHECK( i3 == ci4 );
  CHECK( i4 == ci5 );
  CHECK( i5 == ci0 ); // not incremented (impossible)

  CHECK( d0 == 0 );
  CHECK( d1 == 1 );
  CHECK( d2 == 0 );
  CHECK( d3 == 1 );
  CHECK( d4 == 0 );
  CHECK( d5 == 2 );
}


//TEST_CASE("multi_index_fortran_order") {
//  using MI = multi_index<int,2>;
//  MI dims = {3,2};
//  CHECK( multi_index_fortran_order(dims,0) == MI{0,0} );
//  CHECK( multi_index_fortran_order(dims,1) == MI{1,0} );
//  CHECK( multi_index_fortran_order(dims,2) == MI{2,0} );
//  CHECK( multi_index_fortran_order(dims,3) == MI{0,1} );
//  CHECK( multi_index_fortran_order(dims,4) == MI{1,1} );
//  CHECK( multi_index_fortran_order(dims,5) == MI{2,1} );
//}
//TEST_CASE("multi_index_c_order") {
//  using MI = multi_index<int,2>;
//  MI dims = {3,2};
//  ELOG(multi_index_c_order(dims,0));
//  ELOG(multi_index_c_order(dims,1));
//  ELOG(multi_index_c_order(dims,2));
//  ELOG(multi_index_c_order(dims,3));
//  ELOG(multi_index_c_order(dims,4));
//  ELOG(multi_index_c_order(dims,5));
//  CHECK( multi_index_c_order(dims,0) == MI{0,0} );
//  CHECK( multi_index_c_order(dims,1) == MI{0,1} );
//  CHECK( multi_index_c_order(dims,2) == MI{1,0} );
//  CHECK( multi_index_c_order(dims,3) == MI{1,1} );
//  CHECK( multi_index_c_order(dims,4) == MI{2,0} );
//  CHECK( multi_index_c_order(dims,5) == MI{2,1} );
//}
