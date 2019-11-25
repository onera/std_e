#pragma once


#include "graph/adjacency_graph/io_adjacency.hpp"
#include "std_e/utils/to_string.hpp"


namespace graph {


// TODO ENCAPS
template<class T>
class io_graph {
  public:
  // Class invariant: represent a self-contained io_graph
  // type traits
    using adjacency_type = io_adjacency<T>;
  // ctors
    constexpr io_graph() = default;

    constexpr io_graph(size_t sz)
      : adjs(sz)
    {}

    constexpr
    io_graph(const io_index_adjacency_list<T>& idx_l)
      : adjs(idx_l.size())
    {
      int sz = idx_l.size();
      auto start = &adjs[0];
      for (int i=0; i<sz; ++i) {
        adjs[i] = io_adjacency(idx_l[i],start,i);
      }
    }

    constexpr io_graph(const io_graph& old)
      : adjs(old.adjs.size())
    {
      int sz = adjs.size();
      auto old_start = &old.adjs[0];
      auto start = &adjs[0];
      for (int i=0; i<sz; ++i) {
        adjs[i] = io_adjacency(old.adjs[i],old_start,start,i);
      }
    }
    constexpr io_graph& operator=(const io_graph& old) {
      int sz = old.adjs.size();
      adjs.resize(sz);
      auto old_start = &old.adjs[0];
      auto start = &adjs[0];
      for (int i=0; i<sz; ++i) {
        adjs[i] = io_adjacency(old.adjs[i],old_start,start,i);
      }
      return *this;
    }

    constexpr io_graph(io_graph&& old) = delete; // TODO default in C++20 (constexpr std::vector)
    constexpr io_graph& operator=(io_graph&&) = delete; // TODO default in C++20 (constexpr std::vector)

    constexpr auto
    size() const -> size_t {
      return adjs.size();
    }
    constexpr auto
    begin() -> io_adjacency_iterator<T> {
      //return {adjs.begin(),0};
      return adjs.begin();
    }
    constexpr auto
    begin() const -> const_io_adjacency_iterator<T> {
      //return {adjs.begin(),0};
      return adjs.begin();
    }
    constexpr auto
    end() -> io_adjacency_iterator<T> {
      //return {adjs.begin(),size()};
      return adjs.end();
    }
    constexpr auto
    end() const -> const_io_adjacency_iterator<T> {
      //return {adjs.begin(),size()};
      return adjs.end();
    }
    constexpr auto
    operator[](int i) -> io_adjacency<T>& {
      return adjs[i];
    }
    constexpr auto
    operator[](int i) const -> const io_adjacency<T>& {
      return adjs[i];
    }

  private:
  public: // TODO
    io_adjacency_list<T> adjs; // TODO !! push_back => pointer invalidation if std::vector (but ok with deque)
};


template<class T> constexpr auto
operator==(const io_graph<T>& x, const io_graph<T>& y) -> bool {
  int x_sz = x.size();
  int y_sz = y.size();
  if (x_sz != y_sz) return false;

  auto x_start = x.begin();
  auto y_start = y.begin();
  for (int i=0; i<x_sz; ++i) {
    if (!equal(x[i],y[i],x_start,y_start)) return false;
  }
  return true;
}
template<class T> constexpr auto
operator!=(const io_graph<T>& x, const io_graph<T>& y) -> bool {
  return !(x==y);
}


template<class T> auto
to_string(const io_adjacency<T>& x, const io_adjacency<T>* start) -> std::string {
  auto inward_indices = to_indices(x.inwards,start);
  auto outward_indices = to_indices(x.outwards,start);
  using std::to_string;
  return to_string(x.node) + "| lvl:" + to_string(x.height)
    + " outwd=" + std_e::range_to_string(outward_indices)
    + " inwd=" + std_e::range_to_string(inward_indices);
}

template<class T> auto
to_string(const io_graph<T>& x) -> std::string {
  int sz = x.adjs.size();
  auto* start = &x.adjs[0];
  std::string s;
  for (int i=0; i<sz; ++i) {
    s += to_string(x.adjs[i],start) + "\n";
  }
  return s;
}


} // graph
