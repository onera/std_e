#pragma once


#include "std_e/base/macros.hpp"
#include "std_e/meta/pack.hpp"
#include "std_e/meta/types.hpp"


namespace std_e {


// forward declarations {
template<class... Ts> class tuple_impl;
template<class... Ts> class tuple;
// forward declarations }


// tuple_element {
template<size_t I, class T>
struct tuple_element_impl;

template<size_t I, class... Ts>
struct tuple_element_impl<I,tuple_impl<Ts...>> {
  using type = get_type<I,Ts...>;
};
template<size_t I, class... Ts>
struct tuple_element_impl<I,tuple<Ts...>> {
  using type = get_type<I,Ts...>;
};

template<size_t I, class Tuple> using tuple_element = typename tuple_element_impl<I,Tuple>::type;
// tuple_element }


// tuple_impl {
template<>
class tuple_impl<> {
  public:
    constexpr
    tuple_impl() = default;

    constexpr auto
    operator<=>(const tuple_impl<>& x) const = default;
};

template<class T, class... Ts>
class tuple_impl<T,Ts...> {
  private:
  public: // TODO
    [[no_unique_address]] T head;
    [[no_unique_address]] tuple_impl<Ts...> tail;
  public:
    constexpr
    tuple_impl() = default;

    template<class T0, class... Ts0>
      requires ( !std::is_same_v<std::remove_cvref_t<T0>,tuple_impl<T,Ts...>> // do not match instead of copy/move ctors
              && !std::is_same_v<std::remove_cvref_t<T0>,tuple_impl<T,Ts...>>) // do not match ctor just below
        constexpr
    tuple_impl(T0&& x, Ts0&&... xs)
      : head(FWD(x))
      , tail(FWD(xs)...)
    {}

    template<class T0, class... Ts0>
      requires (!std::is_same_v<tuple_impl<T0,Ts0...>,tuple_impl<T,Ts...>>) // do not match instead of copy/move ctors
        constexpr
    tuple_impl(const tuple_impl<T0,Ts0...>& x)
      : head(x.head)
      , tail(x.tail)
    {}

    template<class... Ts0, class... Ts1> friend constexpr auto
    operator==(const tuple_impl<Ts0...>& x, const tuple_impl<Ts1...>& y) -> bool;
    template<class... Ts0, class... Ts1> friend constexpr auto
    operator<(const tuple_impl<Ts0...>& x, const tuple_impl<Ts1...>& y) -> bool;


    template<size_t I, class... Ts0>
      requires (I < sizeof...(Ts0))
        friend constexpr auto
    get_impl(tuple_impl<Ts0...>& x) -> tuple_element<I,tuple_impl<Ts0...>>&;

    template<size_t I, class... Ts0>
      requires (I < sizeof...(Ts0))
        friend constexpr auto
    get_impl(const tuple_impl<Ts0...>& x) -> const tuple_element<I,tuple_impl<Ts0...>>&;
};

template<size_t I, class... Ts0>
  requires (I < sizeof...(Ts0))
    constexpr auto
get_impl(tuple_impl<Ts0...>& x) -> tuple_element<I,tuple_impl<Ts0...>>& {
  if constexpr (I == 0) {
    return x.head;
  } else {
    return get_impl<I-1>(x.tail);
  }
}

template<size_t I, class... Ts0>
  requires (I < sizeof...(Ts0))
    constexpr auto
get_impl(const tuple_impl<Ts0...>& x) -> const tuple_element<I,tuple_impl<Ts0...>>& {
  if constexpr (I == 0) {
    return x.head;
  } else {
    return get_impl<I-1>(x.tail);
  }
}


/// comparisons {
template<class... Ts0, class... Ts1> constexpr auto
operator==(const tuple_impl<Ts0...>& x, const tuple_impl<Ts1...>& y) -> bool {
  return x.head==y.head && x.tail==y.tail;
}
template<class... Ts0, class... Ts1> constexpr auto
operator<(const tuple_impl<Ts0...>& x, const tuple_impl<Ts1...>& y) -> bool {
  return x.head<y.head || (x.head==y.head && x.tail<y.tail);
}
template<class... Ts0, class... Ts1> constexpr auto
operator!=(const tuple_impl<Ts0...>& x, const tuple_impl<Ts1...>& y) -> bool {
  return !(x==y);
}
template<class... Ts0, class... Ts1> constexpr auto
operator>(const tuple_impl<Ts0...>& x, const tuple_impl<Ts1...>& y) -> bool {
  return y<x;
}
template<class... Ts0, class... Ts1> constexpr auto
operator<=(const tuple_impl<Ts0...>& x, const tuple_impl<Ts1...>& y) -> bool {
  return !(y>x);
}
template<class... Ts0, class... Ts1> constexpr auto
operator>=(const tuple_impl<Ts0...>& x, const tuple_impl<Ts1...>& y) -> bool {
  return !(x<y);
}
/// comparisons }
// tuple_impl }


// tuple {


/// same_rm_cvref_tuple {
template<class T0, class T1>
struct same_rm_cvref_tuple_impl {
  static constexpr bool value = false;
};

template<class... Ts0, class... Ts1>
struct same_rm_cvref_tuple_impl<tuple<Ts0...>,tuple<Ts1...>> {
  using Tuple_0 = tuple<std::remove_cvref_t<Ts0>...>;
  using Tuple_1 = tuple<std::remove_cvref_t<Ts1>...>;
  static constexpr bool value = std::is_same_v<Tuple_0,Tuple_1>;
};

template<class T0, class T1> constexpr bool same_rm_cvref_tuple = same_rm_cvref_tuple_impl<std::remove_cvref_t<T0>,std::remove_cvref_t<T1>>::value;
/// same_rm_cvref_tuple }


/// is_std_e_tuple {
template<class T>
struct is_std_e_tuple_impl {
  static constexpr bool value = false;
};

template<class... Ts>
struct is_std_e_tuple_impl<tuple<Ts...>> {
  static constexpr bool value = true;
};

template<class T> constexpr bool is_std_e_tuple = is_std_e_tuple_impl<T>::value;

template<class T> constexpr bool is_std_e_tuple_ref = (    std::is_lvalue_reference_v<T>
                                                       ||  std::is_rvalue_reference_v<T> )
                                                   && is_std_e_tuple<std::remove_cvref_t<T>>;
/// is_std_e_tuple }


// class tuple {
template<class... Ts>
class tuple {
  private:
  // data member
    tuple_impl<Ts...> impl;

  // private functions
    template<size_t I, class Self, class Other> static auto
    assign_through_refs_impl_at(Self&& x, Other&& y) -> void {
      get<I>(FWD(x)) = get<I>(FWD(y));
    }
    template<class Self, class Other, size_t... Is> static auto
    assign_through_refs_impl(Self&& x, Other&& y, std::index_sequence<Is...>) -> void {
      ( assign_through_refs_impl_at<Is>(FWD(x),FWD(y)) , ... );
    }

    template<class Self, class Other> static auto
    assign_through_refs(Self&& x, Other&& y) -> void {
      assign_through_refs_impl(FWD(x),FWD(y),std::make_index_sequence<sizeof...(Ts)>{});
    }
  public:
  // friends
    template<class... Ts0> friend class tuple;
  // ctors
    constexpr tuple() = default;
    constexpr tuple(const tuple& x) = default;
    constexpr tuple(tuple&& x) = default;

    template<class T0, class... Ts0>
      requires ( !std::is_same_v< std::remove_cvref_t<T0> , tuple<Ts...> > // do not match instead of copy/move ctors
              && !is_std_e_tuple<T0> ) // do not match ctor just below
        constexpr
    tuple(T0&& x, Ts0&&... xs)
      : impl(FWD(x),FWD(xs)...)
    {}

    template<class... Ts0>
      requires (!std::is_same_v<tuple<Ts0...>,tuple<Ts...>>) // do not match instead of copy/move ctors
        constexpr
    tuple(const tuple<Ts0...>& x)
      : impl(x.impl)
    {}

  // assignement
    constexpr tuple& operator=(const tuple& x) {
      assign_through_refs(*this,x);
      return *this;
    }
    constexpr tuple& operator=(tuple&& x) {
      assign_through_refs(*this,std::move(x));
      return *this;
    }
    // We overload operator= even for a const object
    // Indeed, we assign to the value refered to,
    //   but the reference themselves don't change
    //   so the tuple of reference stays the same (shallow constness)
    // This is needed to have std_e::tuple<Ts&...> be a proxy reference type
    //   that works with the std::indirectly writable concept
    template<class... Ts0> constexpr auto
    operator=(const tuple<Ts0...>& x) -> tuple& {
      assign_through_refs(*this,x);
      return *this;
    }
    template<class... Ts0> constexpr auto
    operator=(tuple<Ts0...>&& x) -> tuple& {
      assign_through_refs(*this,std::move(x));
      return *this;
    }
    template<class... Ts0> constexpr auto
    operator=(const tuple<Ts0...>& x) const -> const tuple& {
      assign_through_refs(*this,x);
      return *this;
    }
    template<class... Ts0> constexpr auto
    operator=(tuple<Ts0...>&& x) const -> const tuple& {
      assign_through_refs(*this,std::move(x));
      return *this;
    }

    template<class... Ts0, class... Ts1> friend constexpr auto
    operator==(const tuple<Ts0...>& x, const tuple<Ts1...>& y) -> bool;
    template<class... Ts0, class... Ts1> friend constexpr auto
    operator<(const tuple<Ts0...>& x, const tuple<Ts1...>& y) -> bool;

    template<size_t I, class... Ts0> friend constexpr auto
    get(tuple<Ts0...>& t) -> tuple_element<I,tuple<Ts0...>>&;
    template<size_t I, class... Ts0> friend constexpr auto
    get(const tuple<Ts0...>& t) -> const tuple_element<I,tuple<Ts0...>>&;
};
// class tuple }


/// get {
template<size_t I, class... Ts> constexpr auto
get(tuple<Ts...>& x) -> tuple_element<I,tuple<Ts...>>& {
  return get_impl<I>(x.impl);
}
template<size_t I, class... Ts> constexpr auto
get(const tuple<Ts...>& x) -> const tuple_element<I,tuple<Ts...>>& {
  return get_impl<I>(x.impl);
}
/// get }


/// comparisons {
template<class... Ts0, class... Ts1> constexpr auto
operator==(const tuple<Ts0...>& x, const tuple<Ts1...>& y) -> bool {
  return x.impl == y.impl;
}
template<class... Ts0, class... Ts1> constexpr auto
operator<(const tuple<Ts0...>& x, const tuple<Ts1...>& y) -> bool {
  return x.impl < y.impl;
}
template<class... Ts0, class... Ts1> constexpr auto
operator!=(const tuple<Ts0...>& x, const tuple<Ts1...>& y) -> bool {
  return !(x==y);
}
template<class... Ts0, class... Ts1> constexpr auto
operator>(const tuple<Ts0...>& x, const tuple<Ts1...>& y) -> bool {
  return y<x;
}
template<class... Ts0, class... Ts1> constexpr auto
operator<=(const tuple<Ts0...>& x, const tuple<Ts1...>& y) -> bool {
  return !(y>x);
}
template<class... Ts0, class... Ts1> constexpr auto
operator>=(const tuple<Ts0...>& x, const tuple<Ts1...>& y) -> bool {
  return !(x<y);
}
/// comparisons }

/// swap {
//template<class... Ts, size_t... Is> static auto
//swap_impl(const tuple<Ts&...>& x, const tuple<Ts&...>& y, std::index_sequence<Is...>) -> void {
//  using std::swap;
//  ( swap(get<Is>(x),get<Is>(y)) , ... );
//}
//
//template<class... Ts> constexpr auto
//swap(const tuple<Ts&...>& x, const tuple<Ts&...>& y) -> void {
//  swap_impl(x,y,std::make_index_sequence<sizeof...(Ts)>{});
//}
template<class Tuple, size_t... Is> static auto
swap_impl(Tuple&& x, Tuple&& y, std::index_sequence<Is...>) -> void {
  using std::swap;
  ( swap(get<Is>(x),get<Is>(y)) , ... );
}

template<class... Ts> constexpr auto
swap(tuple<Ts...>& x, tuple<Ts...>& y) -> void {
  swap_impl(x,y,std::make_index_sequence<sizeof...(Ts)>{});
}
// Swapping const tuple<Ts...>& is legit if Ts are reference (or proxy reference) types
// And in this case, we really want to swap member by member (i.e. not use the generic std::swap)
template<class... Ts> constexpr auto
swap(const tuple<Ts...>& x, const tuple<Ts...>& y) -> void {
  swap_impl(x,y,std::make_index_sequence<sizeof...(Ts)>{});
}
template<class... Ts> constexpr auto
swap(tuple<Ts...>&& x, tuple<Ts...>&& y) -> void {
  swap_impl(x,y,std::make_index_sequence<sizeof...(Ts)>{});
}
/// swap }

// tuple }

//
// customization point for reference_value_t {
template<class... Ts>
struct reference_value<std_e::tuple<Ts...>> {
  using type = std_e::tuple< reference_value_t<Ts> ... >;
};
// customization point for reference_value_t }

} // std_e


template<class... Ts>
struct std::tuple_size<std_e::tuple<Ts...>> {
  static constexpr size_t value = sizeof...(Ts);
};


// Customization point for std::common_reference
// SEE https://en.cppreference.com/w/cpp/utility/tuple/basic_common_reference
template<class... Ts, class... Us, template<class> class TQual, template<class> class UQual >
  requires requires { typename std_e::tuple<std::common_reference_t<TQual<Ts>, UQual<Us>>...>; }
struct std::basic_common_reference<std_e::tuple<Ts...>, std_e::tuple<Us...>, TQual, UQual> {
  using type = std_e::tuple<std::common_reference_t<TQual<Ts>, UQual<Us>>...>;
};
