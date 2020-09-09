#pragma once


#include <utility>


namespace std_e {


// DOC REF https://en.wikipedia.org/wiki/Sorting_network
template<class T> auto
sort_network(T& x0, T& x1) -> void {
  if (x1<x0) {
    std::swap(x0,x1);
  }
}
template<class T> auto
sort_network(T& x0, T& x1, T& x2) -> void {
  sort_network(x0, x1);
  sort_network(x1, x2);
  sort_network(x0, x1);
}

template<class T> auto
sort_network(T& x0, T& x1, T& x2, T& x3) -> void {
  sort_network(x0, x1);
  sort_network(x2, x3);
  sort_network(x0, x2);
  sort_network(x1, x3);
  sort_network(x1, x2);
}




template<int N>
struct sorting_network;

template<>
struct sorting_network<0> {
  template<class Random_it> static auto
  sort(Random_it /*first*/) -> void {}
};

template<>
struct sorting_network<1> {
  template<class Random_it> static auto
  sort(Random_it /*first*/) -> void {}
};

template<>
struct sorting_network<2> {
  template<class Random_it> static auto
  sort(Random_it first) -> void {
    sort_network(first[0],first[1]);
  }
};

template<>
struct sorting_network<3> {
  template<class Random_it> static auto
  sort(Random_it first) -> void {
    sort_network(first[0],first[1],first[2]);
  }
};

template<>
struct sorting_network<4> {
  template<class Random_it> static auto
  sort(Random_it first) -> void {
    sort_network(first[0],first[1],first[2],first[3]);
  }
};


} // std_e
