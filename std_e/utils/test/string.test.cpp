#include "std_e/meta/index_sequence.hpp"
#include "std_e/unit_test/doctest.hpp"
#include "std_e/utils/string.hpp"

using namespace std;

TEST_CASE("string utils") {
  SUBCASE("remove_back_n") {
    string s = "abcde";
    std_e::remove_back_n(s,2);

    string expected_s = "abc";
    CHECK( s == expected_s );
  }

  SUBCASE("join") {
    CHECK( std_e::join({"ab","c","d","e"},"") == "abcde" );
    CHECK( std_e::join({"ab","c","d","e"},"|") == "ab|c|d|e" );

    CHECK( std_e::join({}     ,"|") == ""  );
    CHECK( std_e::join({""}   ,"|") == ""  );
    CHECK( std_e::join({"",""},"|") == "|" );
  }
  SUBCASE("split") {
    CHECK( std_e::split("ab|c|d|e",'|') == vector<string>{"ab","c","d","e"} );

    CHECK( std_e::split(""  ,'|') == vector<string>{} );
    CHECK( std_e::split("ab",'|') == vector<string>{"ab"} );
  }

  SUBCASE("balanced_parens") {
    CHECK( std_e::balanced_parens(""    ,'(',')') == true );
    CHECK( std_e::balanced_parens("()"  ,'(',')') == true );
    CHECK( std_e::balanced_parens("(())",'(',')') == true );
    CHECK( std_e::balanced_parens("()()",'(',')') == true );
    CHECK( std_e::balanced_parens("(()" ,'(',')') == false );
    CHECK( std_e::balanced_parens("())" ,'(',')') == false );
    CHECK( std_e::balanced_parens(")(()",'(',')') == false );
  }

  SUBCASE("split_with_parens") {
    CHECK( std_e::split_with_parens("ab(x,y),c(),d,e(x,y(z,z))",',','(',')') == vector<string>{"ab(x,y)","c()","d","e(x,y(z,z))"} );
  }

  SUBCASE("copy_without_spaces") {
    CHECK( std_e::copy_without_spaces("ab c d e") == "abcde" );

    CHECK( std_e::copy_without_spaces("ab") == "ab" );
    CHECK( std_e::copy_without_spaces("")   == "" );
    CHECK( std_e::copy_without_spaces(" ")  == "" );
  }

  SUBCASE("trim") {
    CHECK( std_e::trim("") == "" );
    CHECK( std_e::trim("a b  c") == "a b  c" );
    CHECK( std_e::trim("  a b  c") == "a b  c" );
    CHECK( std_e::trim("a b  c  ") == "a b  c" );
    CHECK( std_e::trim(" a b  c  ") == "a b  c" );
  }

  SUBCASE("remove_spaces_and_split") {
    CHECK( std_e::remove_spaces_and_split("ab | c| d| e  ",'|') == vector<string>{"ab","c","d","e"} );
  }

  SUBCASE("contains char") {
    CHECK(  std_e::contains("abcde",'a') );
    CHECK(  std_e::contains("abcde",'d') );
    CHECK(  std_e::contains("abcde",'e') );
    CHECK( !std_e::contains("abcde",'x') );

    CHECK(  std_e::contains("a",'a') );
    CHECK( !std_e::contains("",'x') );
  }
  SUBCASE("contains string") {
    CHECK(  std_e::contains("abcde","ab") );
    CHECK(  std_e::contains("abcde","cd") );
    CHECK(  std_e::contains("abcde","de") );
    CHECK( !std_e::contains("abcde","ax") );

    CHECK(  std_e::contains("abcde","abcde") );
    CHECK(  std_e::contains("abcde","") );
    CHECK( !std_e::contains("abcde","abcdef") );
  }

  SUBCASE("begins_with") {
    CHECK(  std_e::begins_with("abcde","a") );
    CHECK(  std_e::begins_with("abcde","ab") );
    CHECK(  std_e::begins_with("abcde","abcde") );
    CHECK( !std_e::begins_with("abcde","bc") );

    CHECK( !std_e::begins_with("abcde","abcded") );
    CHECK(  std_e::begins_with("abab","ab") );
    CHECK(  std_e::begins_with("","") );
    CHECK(  !std_e::begins_with(""," ") );
  }
  SUBCASE("ends_with") {
    CHECK(  std_e::ends_with("abcde","e") );
    CHECK(  std_e::ends_with("abcde","de") );
    CHECK(  std_e::ends_with("abcde","abcde") );
    CHECK( !std_e::ends_with("abcde","cd") );

    CHECK( !std_e::ends_with("abcde","zabcde") );
    CHECK(  std_e::ends_with("abab","ab") );
    CHECK(  std_e::ends_with("","") );
    CHECK(  !std_e::ends_with(""," ") );
  }
}
