#pragma once


namespace std_e {


// ror means range of ranges
template<class T, template<class...> class Outer_range, template<class...> class Inner_range = Outer_range>
using range_of_ranges = Outer_range<Inner_range<T>>;

// traits {
/// value_type {
template<class adjacency_list_type>
struct value_type__impl;

template<class T, template<class...> class OR, template<class...> class IR>
struct value_type__impl<range_of_ranges<T,OR,IR>> {
  using type = T;
};
template<class adjacency_list_type>
using value_type = typename value_type__impl<adjacency_list_type>::type;
/// value_type }

/// inner_range_type {
template<class adjacency_list_type>
struct inner_range_type__impl;

template<class T, template<class...> class OR, template<class...> class IR>
struct inner_range_type__impl<range_of_ranges<T,OR,IR>> {
  using type = IR<T>;
};
template<class adjacency_list_type>
using inner_range_type = typename inner_range_type__impl<adjacency_list_type>::type;
/// inner_range_type }

/// range_from_outer {
template<class adjacency_list_type, class T0>
struct range_from_outer__impl;

template<class T, template<class...> class OR, template<class...> class IR, class T0>
struct range_from_outer__impl<range_of_ranges<T,OR,IR>,T0> {
  using type = OR<T0>;
};
template<class adjacency_list_type, class T>
using range_from_outer = typename range_from_outer__impl<adjacency_list_type,T>::type;
/// range_from_outer }

/// range_of_ranges_from {
template<class adjacency_list_type, class T0>
struct range_of_ranges_from__impl;

template<class T, template<class...> class OR, template<class...> class IR, class T0>
struct range_of_ranges_from__impl<range_of_ranges<T,OR,IR>,T0> {
  using type = range_of_ranges<T0,OR,IR>;
};
template<class adjacency_list_type, class T>
using range_of_ranges_from = typename range_of_ranges_from__impl<adjacency_list_type,T>::type;
/// range_of_ranges_from }

// traits }


} // std_e
