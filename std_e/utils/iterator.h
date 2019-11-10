#pragma once


namespace std_e {


  // TODO DEL (std::iterator_traits)
template<class Collection>
struct iterator__impl {
  using type = typename std::decay_t<Collection>::iterator;
};
template<class Collection>
struct iterator__impl<const Collection&> {
  using type = typename Collection::const_iterator;
};
template<class Collection>
struct iterator__impl<const Collection&&> {
  using type = typename Collection::const_iterator;
};

template<class Collection> using iterator = typename iterator__impl<Collection>::type;


} // std_e
