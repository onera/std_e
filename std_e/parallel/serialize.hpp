#pragma once

#include <type_traits>

// TODO replace std::is_trivially_copyable_v by something that also check there is no pointers (not doable now)
// TODO deserialize return the object (instead of taking it by ref)
// TODO deserialize use void* + size, not span


// declarations {
template<class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> =0 > auto
serialize(const T& x);

template<class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> =0 > auto
serialize(const std_e::span<T>& x);

template<class T, std::enable_if_t<!std::is_trivially_copyable_v<T>, int> =0 > auto
serialize(const std_e::span<T>& x) -> std::vector<std::byte>;

template<class T, ptrdiff_t N, std::enable_if_t<std::is_trivially_copyable_v<T>, int> =0 > auto
deserialize(std_e::span<const std::byte,N> x, T& out) -> void;

template<class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> =0 > auto
deserialize(std_e::span<const std::byte> x, std::vector<T>& out) -> void;

template<class T, std::enable_if_t<!std::is_trivially_copyable_v<T>, int> =0 > auto
deserialize(std_e::span<const std::byte> x, std::vector<T>& out) -> void;

template<class T> auto
serialize(const std::vector<T>& x);
// declarations }


// trivial types and arrays of them {
/// serialize {
/** NOTE: for these types, there is actually nothing to do. In particular, no need to copy memory */
template<class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> =0 > auto
serialize(const T& x) {
  auto ptr = (const std::byte*)&x;
  return std_e::span<const std::byte,sizeof(T)>(ptr);
}

/** no need to own memory to serialize */
template<class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> =0 > auto
serialize(const std_e::span<T>& x) {
  return std_e::span<const std::byte>((std::byte*)x.data(),x.size()*sizeof(T));
}
/// serialize }

/// deserialize {
/** NOTE: for efficient deserialization of trivial type, and array of them,
 *        we could just ask to fill the serialized bits into the final object
 *        since there is no need to do anything
 *        SEE the deserialize() overloads with a function as first argument instead of a span
*/
template<class T, ptrdiff_t N, std::enable_if_t<std::is_trivially_copyable_v<T>, int> =0 > auto
deserialize(std_e::span<const std::byte,N> x, T& out) -> void {
  auto ptr = (const T*)x.data();
  out = *ptr;
}

template<class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> =0 > auto
deserialize(std_e::span<const std::byte> x, std::vector<T>& out) -> void {
  int vector_size = x.size()/sizeof(T);
  out.resize(vector_size);
  auto ptr = (T*)x.data();
  std::copy_n(ptr,vector_size,out.data());
}
/// deserialize }

// trivial }




// non trivial {

template<class T, std::enable_if_t<!std::is_trivially_copyable_v<T>, int> =0 > auto
serialize(const std_e::span<T>& x) -> std::vector<std::byte> {
  int nb_elts = x.size();
  std::vector<std::byte> res((1+nb_elts+1)*sizeof(int));

   // position zero (section A) holds the number of elements
  auto nb_elts_position = (int*)res.data();
  *nb_elts_position = nb_elts;

  // positions in [1,nb_elts+1) (section B) hold the offset of each serialized object
  int* offset_position = (int*)res.data()+1;

  int offset = 0;
  for (int i=0; i<nb_elts; ++i) {
    auto elt_serial = serialize(x[i]);
    *((int*)res.data()+1+i)= offset;
    ++offset_position;
    append(res,elt_serial); // the rest of the serialized array (section C) holds the concatenated data of each of its elements
    offset += elt_serial.size();
  }
  *((int*)res.data()+1+nb_elts)= offset;
  return res;
}

template<class T, std::enable_if_t<!std::is_trivially_copyable_v<T>, int> =0 > auto
deserialize(std_e::span<const std::byte> x, std::vector<T>& out) -> void {
  // section (A) holds holds the number of elements
  auto nb_elts_position = (int*)x.data();
  int nb_elts = *nb_elts_position;

  // section (B) holds the offset of each serialized object
  int* offset_position = (int*)x.data()+1;

  // section (C) the rest of the serialized array holds the concatenated data of each of its elements
  const std::byte* begin_data = x.data() + (1+nb_elts+1)*sizeof(int);

  out.resize(nb_elts);
  for (int i=0; i<nb_elts; ++i) {
    int size = *(offset_position+1)-(*offset_position);
    const std::byte* data_ptr = begin_data+*offset_position++;
    deserialize(std_e::make_span(data_ptr,size),out[i]);
  }

  int section_A_size = sizeof(int);
  int section_B_size = (nb_elts+1)*sizeof(int);
  int section_C_size = *offset_position;
  STD_E_ASSERT(section_A_size+section_B_size+section_C_size==(int)x.size());
}

// non trivial }



// In all cases (trivial or not), serializing a Contiguous_range is the same as serializing a span of it
template<class T> auto
serialize(const std::vector<T>& x) {
  return serialize(std_e::make_span(x));
}


// deserialize from function {

template<class F, class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> =0 > auto
deserialize(F f, int size, T& x) -> void {
  auto ptr = (std::byte*)(&x);
  f(ptr,size);
}

template<class F, class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> =0 > auto
deserialize(F f, int size, std::vector<T>& x) -> void {
  x.resize(size/sizeof(int));
  auto ptr = (std::byte*)x.data();
  f(ptr,size);
}

template<class F, class T, std::enable_if_t<!std::is_trivially_copyable_v<T>, int> =0 > auto
deserialize(F f, int size, T& x) -> void {
  std::vector<std::byte> buf(size);
  f(buf.data(),size);
  deserialize(std_e::make_span(buf),x);
}

// deserialize from function }
