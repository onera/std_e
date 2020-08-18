#pragma once

#include <type_traits>
#include <vector>
#include "std_e/future/span.hpp"
#include "std_e/utils/type_traits.hpp"
#include <iostream> // TODO

namespace std_e {

// TODO replace std::is_trivially_copyable_v by something that also check there is no pointers (not doable now)
// TODO deserialize return the object (instead of taking it by ref)


// declarations {
template<class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> =0 > auto
serialize(const T& x);

template<class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> =0 > auto
serialize(const span<T>& x);

template<class T, std::enable_if_t<!std::is_trivially_copyable_v<T>, int> = 0 > auto
serialize_array(const span<T>& x) -> std::pair<std::vector<int>,std::vector<std::byte>>;
template<class T, std::enable_if_t<!std::is_trivially_copyable_v<T>, int> =0 > auto
serialize(const span<T>& x) -> std::vector<std::byte>;

template<class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> =0 > auto
deserialize(const std::byte* v_ptr, int n, T& out) -> void;

template<class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> =0 > auto
deserialize(const std::byte* v_ptr, int n, std::vector<T>& out) -> void;

template<class T, std::enable_if_t<!std::is_trivially_copyable_v<T>, int> =0 > auto
deserialize(const std::byte* v_ptr, int n, std::vector<T>& out) -> void;

template<class T> auto
serialize(const std::vector<T>& x);
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
deserialize(const std::byte* v_ptr, int n, T& out) -> void {
  STD_E_ASSERT(n==sizeof(T));
  auto ptr = (const T*)v_ptr;
  out = *ptr;
}

template<class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int>> auto
deserialize(const std::byte* v_ptr, int n, std::vector<T>& out) -> void {
  int vector_size = n/sizeof(T);
  out.resize(vector_size);
  auto ptr = (const T*)v_ptr;
  std::copy_n(ptr,vector_size,out.data());
}

template<class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> =0> auto
deserialize_to_span(const std::byte* v_ptr, int n) -> std_e::span<const T> {
  return std_e::make_span((const T*)v_ptr,n);
}
template<class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> =0> auto
deserialize_to_span(std::byte* v_ptr, int n) -> std_e::span<T> {
  return std_e::make_span((T*)v_ptr,n);
}
/// deserialize }

// trivial }




// non trivial {

template<class T, std::enable_if_t<!std::is_trivially_copyable_v<T>, int>> auto
serialize_array(const span<T>& x) -> std::pair<std::vector<int>,std::vector<std::byte>> {
  int nb_elts = x.size();
  std::vector<int> offsets(nb_elts+1);
  std::vector<std::byte> data;

  int offset = 0;
  for (int i=0; i<nb_elts; ++i) {
    offsets[i] = offset;
    auto elt_data = serialize(x[i]);
    append(data,elt_data);
    offset += elt_data.size();
  }
  offsets.back() = offset;
  return std::make_pair(offsets,data);
}

template<class T, std::enable_if_t<!std::is_trivially_copyable_v<T>, int> > auto
serialize(const span<T>& x) -> std::vector<std::byte> {
  auto [offsets,data] = serialize_array(x);
  int nb_elts = offsets.size()-1;
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

// TODO Knot_sequence
template<class T, class Random_access_integer_range, std::enable_if_t<!std::is_trivially_copyable_v<T>, int> =0> auto
deserialize_array(const std::byte* v_ptr, const Random_access_integer_range& offsets) -> std::vector<T> {
  const int nb_elts = offsets.size()-1;

  std::vector<T> res(nb_elts);
  for (int i=0; i<nb_elts; ++i) {
    int size = offsets[i+1]-offsets[i];
    const std::byte* elt_ptr = v_ptr+offsets[i];
    deserialize(elt_ptr,size,res[i]);
  }

  return res;
}
template<class T, std::enable_if_t<!std::is_trivially_copyable_v<T>, int> > auto
deserialize(const std::byte* v_ptr, int n, std::vector<T>& out) -> void {
  // section (A) holds holds the number of elements
  const int* nb_elts_position = (const int*)v_ptr;
  const int nb_elts = *nb_elts_position;

  // section (B) holds the offset of each serialized object
  auto offsets_start = v_ptr + sizeof(int);
  auto offsets = deserialize_to_span<int>(offsets_start,nb_elts+1);

  // section (C) the rest of the serialized array holds the concatenated data of each of its elements
  auto data_start = v_ptr + (1+nb_elts+1)*sizeof(int);
  out = deserialize_array<T>(data_start,offsets);

  int section_A_size = sizeof(int);
  int section_B_size = (nb_elts+1)*sizeof(int);
  //int section_C_size = offsets.length();
  int section_C_size = offsets.back(); // TODO DEL, use above
  STD_E_ASSERT(section_A_size+section_B_size+section_C_size==n);
}

// non trivial }



// In all cases (trivial or not), serializing a Contiguous_range is the same as serializing a span of it
template<class T> auto
serialize(const std::vector<T>& x) {
  return serialize(make_span(x));
}


// deserialize from function {

template<class F, class T, std::enable_if_t<std_e::is_callable<F> && std::is_trivially_copyable_v<T>, int> = 0> auto
deserialize(F f, int size, T& x) -> void {
  auto ptr = (std::byte*)(&x);
  f(ptr,size);
}

template<class F, class T, std::enable_if_t<std_e::is_callable<F> && std::is_trivially_copyable_v<T>, int> = 0> auto
deserialize(F f, int size, std::vector<T>& x) -> void {
  x.resize(size/sizeof(int));
  auto ptr = (std::byte*)x.data();
  f(ptr,size);
}

template<class F, class T, std::enable_if_t<std_e::is_callable<F> && !std::is_trivially_copyable_v<T>, int> = 0> auto
deserialize(F f, int size, T& x) -> void {
  std::vector<std::byte> buf(size);
  f(buf.data(),size);
  deserialize(buf.data(),buf.size(),x);
}

// deserialize from function }

} // std_e
