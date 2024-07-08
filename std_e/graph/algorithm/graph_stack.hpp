#pragma once

#include <vector>


namespace std_e {


// TODO look if algo using this structure could be re-formulated using depth-first build
template<class T>
class graph_stack {
  public:
  // ctors
    constexpr
    graph_stack() {}

    constexpr
    graph_stack(T root_value)
      : S{std::move(root_value)}
    {}

  // basic query
    constexpr auto
    size() const -> int {
      return S.size();
    }
    constexpr auto
    is_valid() const -> bool {
      return size()>0;
    }
    constexpr auto
    is_at_root_level() const -> bool {
      return size()==1;
    }

  // stack functions
    constexpr auto
    push_level(const T& x) -> void {
      S.push_back(x);
    }
    constexpr auto
    pop_level() -> void {
      S.pop_back();
    }

  // accessors
    constexpr auto
    current_level() -> T& {
      STD_E_ASSERT(is_valid());
      return S[S.size()-1];
    }
    constexpr auto
    current_level() const -> const T& {
      STD_E_ASSERT(is_valid());
      return S[S.size()-1];
    }
    constexpr auto
    parent_level() -> T& {
      STD_E_ASSERT(is_valid() && !is_at_root_level());
      return S[S.size()-2];
    }
    constexpr auto
    parent_level() const -> const T& {
      STD_E_ASSERT(is_valid() && !is_at_root_level());
      return S[S.size()-2];
    }

  private:
    std::vector<T> S;
};


} // std_e
