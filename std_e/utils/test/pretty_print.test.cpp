//#include "std_e/unit_test/doctest.hpp"
//#include "std_e/utils/pretty_print.hpp"
//
//
// TODO crashes on some compilers...
//TEST_CASE("pretty_print_bytes") {
//  CHECK( std_e::pretty_print_bytes(   1) == "1 B" );
//  CHECK( std_e::pretty_print_bytes( 128) == "128 B" );
//  CHECK( std_e::pretty_print_bytes(1023) == "1023 B" );
//  //CHECK( std_e::pretty_print_bytes(1024) == "1 KB" );  TODO "1024B" on some compilers...
//  CHECK( std_e::pretty_print_bytes(1025) == "1.00098 KB" );
//  CHECK( std_e::pretty_print_bytes(1024*1024) == "1 MB" );
//  CHECK( std_e::pretty_print_bytes(1024*1024 + 1024) == "1.00098 MB" );
//  CHECK( std_e::pretty_print_bytes(1024*1024*1024) == "1 GB" );
//  CHECK( std_e::pretty_print_bytes(1024*1024*1024*1024l) == "1 TB" );
//  CHECK( std_e::pretty_print_bytes(1024*1024*1024*1024l*1024l) == "1024 TB" );
//}
