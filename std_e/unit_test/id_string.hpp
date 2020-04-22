#pragma once


#include <string>


// simple type, rich enough to unit test many things
struct id_string {
  int id;
  std::string s;
};
constexpr auto
operator==(const id_string& x, const id_string& y) -> bool {
  return x.id==y.id && x.s==y.s;
}
constexpr auto
operator!=(const id_string& x, const id_string& y) -> bool {
  return !(x==y);
}
constexpr auto
operator<(const id_string& x, const id_string& y) -> bool {
  return x.id<y.id || (x.id==y.id && x.s<y.s);
}

constexpr auto
equal_ids(const id_string& x, const id_string& y) -> bool {
  return x.id==y.id;
}
constexpr auto
less_ids(const id_string& x, const id_string& y) -> bool {
  return x.id<y.id;
}

inline auto
append_to_string(std::string& s, const id_string& x) -> std::string& {
  s += x.s;
  return s;
};

inline auto
to_string(const id_string& x) -> std::string {
  return "{"+std::to_string(x.id)+","+x.s+"}";
};
