#pragma once

#include <type_traits>
#include <vector>
#include "std_e/future/span.hpp"
#include "std_e/meta/type_traits.hpp"
#include "std_e/interval/knot_sequence.hpp"
#include "std_e/future/contract.hpp"
#include "std_e/algorithm/iota.hpp"
#include "std_e/utils/concatenate.hpp"

namespace std_e {

/*
  concept Trivially_copyable_value {
    Trivially_copyable
    contains no pointer
  }

  concept Trivially_serializable = Trivially_copyable_value || Array<Trivially_copyable_value>
}
*/

//template<class T> constexpr bool std::is_trivially_copyable_v = std::is_trivially_copyable_v<T>;// TODO check there is no pointers (not doable in C++ <= 20)


struct serialized_array { // TODO invert field
  int_knot_vector offsets;
  std::vector<std::byte> data;
};


// declarations {
template<class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> =0 > auto
serialize(const T& x);

template<class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> =0 > auto
serialize(const span<T>& x);

template<class T, std::enable_if_t<!std::is_trivially_copyable_v<T>, int> =0 > auto
serialize(const span<T>& x) -> std::vector<std::byte>;

template<class T> auto
serialize(const std::vector<T>& x);

inline auto
serialize(const std::string& x);


template<class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> =0 > auto
deserialize_into(const std::byte* v_ptr, int n, T& out) -> void;

template<class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> =0 > auto
deserialize_into(const std::byte* v_ptr, int n, std::vector<T>& out) -> void;

template<class T, std::enable_if_t<!std::is_trivially_copyable_v<T>, int> =0 > auto
deserialize_into(const std::byte* v_ptr, int n, std::vector<T>& out) -> void;

inline auto
deserialize_into(const std::byte* v_ptr, int n, std::string& out) -> void;



template<class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0 > auto
serialize_array(const span<T>& x) -> serialized_array;
template<class T, std::enable_if_t<!std::is_trivially_copyable_v<T>, int> = 0 > auto
serialize_array(const span<T>& x) -> serialized_array;
// declarations }


// trivial types and arrays of them {
/// serialize {
/** NOTE: for these types, there is actually nothing to do. In particular, no need to copy memory */
template<class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int>> auto
serialize(const T& x) {
  auto ptr = (const std::byte*)&x;
  return span<const std::byte,sizeof(T)>(ptr);
}

/** no need to own memory to serialize */
template<class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int>> auto
serialize(const span<T>& x) {
  return span<const std::byte>((std::byte*)x.data(),x.size()*sizeof(T));
}
/// serialize }

/// deserialize {
/** NOTE: for efficient deserialization of trivial type, and array of them,
 *        we could just ask to fill the serialized bits into the final object
 *        since there is no need to do anything
 *        SEE the deserialize() overloads with a function as first argument instead of a span
*/
template<class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int>> auto
deserialize_into(const std::byte* v_ptr, int n, T& out) -> void {
  STD_E_ASSERT(n==sizeof(T));
  auto ptr = (const T*)v_ptr;
  out = *ptr;
}

template<class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int>> auto
deserialize_into(const std::byte* v_ptr, int n, std::vector<T>& out) -> void {
  int vector_size = n/sizeof(T);
  out.resize(vector_size);
  auto ptr = (const T*)v_ptr;
  std::copy_n(ptr,vector_size,out.data());
}
inline auto
deserialize_into(const std::byte* v_ptr, int n, std::string& out) -> void {
  out.resize(n);
  auto ptr = (const char*)v_ptr;
  std::copy_n(ptr,n,out.data());
}

template<class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> =0> auto
deserialize_to_span(const std::byte* v_ptr, int n) -> std_e::span<const T> {
  return std_e::make_span((const T*)v_ptr,n);
}

template<class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> =0> auto
deserialize_to_knot_span(const std::byte* v_ptr, int n) -> std_e::knot_span<const T> {
  return to_knot_span(deserialize_to_span<T>(v_ptr,n+1));
}
/// deserialize }

// trivial }


// TODO should return something lighter
template<class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int>> auto
serialize_array(const span<T>& x) -> serialized_array {
  auto serial_view = serialize(x);
  std::vector<std::byte> data(serial_view.begin(),serial_view.end());

  int nb_elts = x.size();
  std_e::int_knot_vector offsets(nb_elts);
  std_e::exclusive_iota_n(begin(offsets),offsets.size(),0,(int)sizeof(T));

  return {offsets,data};
}
// TODO does not really makes sense, since the offset part is not used (but for genericity...)
template<class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> =0> auto
deserialize_array(const serialized_array& x) -> std::vector<T> {
  std::vector<T> res;
  deserialize_into(x.data.data(),x.data.size(),res);
  return res;
}


// non trivial {

template<class T, std::enable_if_t<!std::is_trivially_copyable_v<T>, int>> auto
serialize_array(const span<T>& x) -> serialized_array {
  int nb_elts = x.size();
  std_e::int_knot_vector offsets(nb_elts);
  std::vector<std::byte> data;

  int offset = 0;
  for (int i=0; i<nb_elts; ++i) {
    offsets[i] = offset;
    auto elt_data = serialize(x[i]);
    append(data,elt_data);
    offset += elt_data.size();
  }
  offsets.back() = offset;
  return {offsets,data};
}
template<class T> auto
serialize_array(const std::vector<T>& x) -> serialized_array {
  return serialize_array(make_span(x));
}

template<class T, std::enable_if_t<!std::is_trivially_copyable_v<T>, int> > auto
serialize(const span<T>& x) -> std::vector<std::byte> {
  auto [offsets,data] = serialize_array(x);
  int nb_elts = offsets.nb_intervals();
  int serialized_size = (1+nb_elts+1)*sizeof(int) + data.size();
  std::vector<std::byte> res(serialized_size);

   // position zero (section A) holds the number of elements
  int* nb_elts_position = (int*)res.data();
  *nb_elts_position = nb_elts;

  // positions in [1,nb_elts+1) (section B) hold the offset of each serialized object
  int* offset_position = (int*)res.data()+1;
  std::copy(begin(offsets),end(offsets),offset_position);

  // rest of the serialized array (section C) holds the concatenated data of each of its elements
  std::byte* data_position = res.data() + (1+nb_elts+1)*sizeof(int);
  std::copy(begin(data),end(data),data_position);
  return res;
}

template<class T, class Knot_sequence, std::enable_if_t<!std::is_trivially_copyable_v<T>, int> =0> auto
deserialize_array_into(const std::byte* v_ptr, const Knot_sequence& offsets, std::vector<T>& out) -> void {
  const int nb_elts = offsets.nb_intervals();

  out.resize(nb_elts);
  for (int i=0; i<nb_elts; ++i) {
    const std::byte* elt_ptr = v_ptr+offsets[i];
    deserialize_into(elt_ptr,offsets.length(i),out[i]);
  }
}
template<class T, class Knot_sequence, std::enable_if_t<!std::is_trivially_copyable_v<T>, int> =0> auto
deserialize_array(const std::byte* v_ptr, const Knot_sequence& offsets) -> std::vector<T> {
  std::vector<T> res;
  deserialize_array_into(v_ptr,offsets,res);
  return res;
}
template<class T, std::enable_if_t<!std::is_trivially_copyable_v<T>, int> =0> auto
deserialize_array(const serialized_array& x) -> std::vector<T> {
  return deserialize_array<T>(x.data.data(),x.offsets);
}

template<class T, std::enable_if_t<!std::is_trivially_copyable_v<T>, int> > auto
deserialize_into(const std::byte* v_ptr, int n, std::vector<T>& out) -> void {
  // section (A) holds holds the number of elements
  const int* nb_elts_position = (const int*)v_ptr;
  const int nb_elts = *nb_elts_position;

  // section (B) holds the offset of each serialized object
  auto offsets_start = v_ptr + sizeof(int);
  auto offsets = deserialize_to_knot_span<int>(offsets_start,nb_elts);

  // section (C) the rest of the serialized array holds the concatenated data of each of its elements
  auto data_start = v_ptr + (1+nb_elts+1)*sizeof(int);
  deserialize_array_into<T>(data_start,offsets,out);

  int section_A_size = sizeof(int);
  int section_B_size = (nb_elts+1)*sizeof(int);
  int section_C_size = offsets.length();
  STD_E_ASSERT(section_A_size+section_B_size+section_C_size==n);
}

// non trivial }



// In all cases (trivial or not), serializing a Contiguous_range is the same as serializing a span of it
template<class T> auto
serialize(const std::vector<T>& x) {
  return serialize(make_span(x));
}
auto
serialize(const std::string& x) {
  return serialize(make_span(x.data(),x.size()));
}


// deserialize from function {
template<class F, class T, std::enable_if_t<std_e::is_callable<F> && std::is_trivially_copyable_v<T>, int> = 0> auto
deserialize_into(F f, int size, T& x) -> void {
  auto ptr = (std::byte*)(&x);
  f(ptr,size);
}

template<class F, class T, std::enable_if_t<std_e::is_callable<F> && std::is_trivially_copyable_v<T>, int> = 0> auto
deserialize_into(F f, int size, std::vector<T>& x) -> void {
  x.resize(size/sizeof(int));
  auto ptr = (std::byte*)x.data();
  f(ptr,size);
}

template<class F, class T, std::enable_if_t<std_e::is_callable<F> && !std::is_trivially_copyable_v<T>, int> = 0> auto
deserialize_into(F f, int size, T& x) -> void {
  std::vector<std::byte> buf(size);
  f(buf.data(),size);
  deserialize_into(buf.data(),buf.size(),x);
}


template<class T, class X> auto
deserialize(X x, int size) -> T {
  T res;
  deserialize_into(x,size,res);
  return res;
}
// deserialize from function }

} // std_e
