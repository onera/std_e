#pragma once


#include "std_e/graph/adjacency_graph/io_adjacency.hpp"
#include "std_e/utils/to_string.hpp"


namespace graph {


template<class T>
class io_graph {
  public:
  // Class invariant: represent a self-contained io_graph
  // type traits
    using adjacency_type = io_adjacency<T>;
    using node_type = T;
    using node_adj_type = adjacency_type; // TODO DEL
  // ctors
    constexpr io_graph() = default;

    constexpr io_graph(size_t sz)
      : adjs(sz)
    {}

    constexpr
    io_graph(const io_index_adjacency_vector<T>& idx_adjs)
      : adjs(idx_adjs.size())
    {
      auto start = this->begin_ptr();
      std::transform(idx_adjs.begin(),idx_adjs.end(),adjs.begin(),[start](const auto& idx_adj){
        return io_adjacency(idx_adj,start);
      });
    }

    constexpr io_graph(const io_graph& old)
      : adjs(old.adjs.size())
    {
      auto old_start = old.begin();
      auto start = this->begin();
      std::transform(old.begin(),old.end(),adjs.begin(),[=](const auto& adj){
        return io_adjacency(adj,old_start,start);
      });
    }
    constexpr io_graph& operator=(const io_graph& old) {
      adjs.resize(old.size());
      auto old_start = old.begin();
      auto start = this->begin();
      std::transform(old.begin(),old.end(),adjs.begin(),[=](const auto& adj){
        return io_adjacency(adj,old_start,start);
      });
      return *this;
    }

    constexpr io_graph(io_graph&& old) = default;
    constexpr io_graph& operator=(io_graph&&) = default;

    constexpr auto
    size() const -> size_t {
      return adjs.size();
    }
    constexpr auto
    begin() -> io_adjacency<T>* {
      return adjs.data();
    }
    constexpr auto
    begin() const -> const io_adjacency<T>* {
      return adjs.data();
    }
    constexpr auto
    end() -> io_adjacency<T>* {
      return adjs.data() + size();
    }
    constexpr auto
    end() const -> const io_adjacency<T>* {
      return adjs.data() + size();
    }
    constexpr auto
    begin_ptr() -> io_adjacency<T>* {
      return begin();
    }
    constexpr auto
    begin_ptr() const -> const io_adjacency<T>* {
      return begin();
    }
    //constexpr auto
    //begin() {
    //  return adjs.begin();
    //}
    //constexpr auto
    //begin() const {
    //  return adjs.begin();
    //}
    //constexpr auto
    //end() {
    //  return adjs.end();
    //}
    //constexpr auto
    //end() const {
    //  return adjs.end();
    //}
    //constexpr auto
    //begin_ptr() -> io_adjacency<T>* {
    //  return &*adjs.begin();
    //}
    //constexpr auto
    //begin_ptr() const -> const io_adjacency<T>* {
    //  return &*adjs.begin();
    //}

    constexpr auto
    operator[](int i) -> io_adjacency<T>& {
      return adjs[i];
    }
    constexpr auto
    operator[](int i) const -> const io_adjacency<T>& {
      return adjs[i];
    }
    auto
    push_back(const io_adjacency<T>& x) -> void {
      return adjs.push_back(x); // TODO !!! if vector capacity == size, reallocates, invalidates everything => BAD
    }
    auto
    emplace_back(io_adjacency<T>&& x) -> io_adjacency<T>& {
      return adjs.emplace_back(std::move(x)); // TODO !!! if vector capacity == size, reallocates, invalidates everything => BAD
    }

  private:
  public: // TODO
    io_adjacency_vector<T> adjs;
    //io_adjacency_deque<T> adjs; // use for stronger iterator stability (especially for push_back)
};


template<class T> constexpr auto
begin(io_graph<T>& x) {
  return x.begin();
}
template<class T> constexpr auto
begin(const io_graph<T>& x) {
  return x.begin();
}
template<class T> constexpr auto
end(io_graph<T>& x) {
  return x.end();
}
template<class T> constexpr auto
end(const io_graph<T>& x) {
  return x.end();
}
template<class T> constexpr auto
operator==(const io_graph<T>& x, const io_graph<T>& y) -> bool {
  int x_sz = x.size();
  int y_sz = y.size();
  if (x_sz != y_sz) return false;

  auto x_start = x.begin_ptr();
  auto y_start = y.begin_ptr();
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
  auto inward_indices = std_e::iterators_to_indices(x.inwards,start);
  auto outward_indices = std_e::iterators_to_indices(x.outwards,start);
  using std::to_string;
  return to_string(x.node)
    + " outwd=" + std_e::range_to_string(outward_indices)
    + " inwd=" + std_e::range_to_string(inward_indices);
}

template<class T> auto
to_string(const io_graph<T>& x) -> std::string {
  std::string s;
  for (const auto& adj : x) {
    s += to_string(adj,begin(x)) + "\n";
  }
  return s;
}


// algorithm {
template<class T> constexpr auto
make_bidirectional_from_outward_edges(io_graph<T>& g) {
  for (auto& adj : g) {
    for (auto* out : adj.outwards) {
      out->inwards.push_back( &adj );
    }
  }
}
// algorithm }


} // graph
