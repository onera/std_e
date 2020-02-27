#pragma once


namespace graph {
  template<class T> struct bidirectional_adj;
  template<class T> class bidirectional_graph;
}


namespace doctest {
  template<class T> struct StringMaker<graph::bidirectional_adj<T>> {
    static String convert(const graph::bidirectional_adj<T>& value) {
      //return std_e::range_to_string(value);
      std::string s = to_string(value);
      return s.c_str();
    }
  };
  
  template<class T> struct StringMaker<graph::bidirectional_graph<T>> {
    static String convert(const graph::bidirectional_graph<T>& value) {
      //return std_e::range_to_string(value);
      std::string s = to_string(value);
      return s.c_str();
    }
  };
} // doctest
