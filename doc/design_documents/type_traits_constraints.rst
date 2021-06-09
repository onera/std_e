.. _type_traits_constraints:

Type traits constraints
"""""""""""""""""""""""

In order to constrain template parameters without language support for concepts, we need to combine :cpp:`std::enable_if` and type traits. 

Let us see an example with the :cpp:`Array` concept. The semantic is the following: a type :cpp:`T` models the :cpp:`Array` concept if and only if :cpp:`is_array<T> == true`.

The :cpp:`is_array` type trait is defined as follow:

.. code:: c++

  template<class T> struct enable_is_array : std::false_type {};
  template<class T> constexpr bool is_array = enable_is_array<T>::value;

By default, types do not verify :cpp:`is_array`. We can **enable** the type trait for specific types by specializing :cpp:`enable_is_array`:

.. code:: c++

  template<class T, size_t N>
  struct enable_is_array<std::array<T,N>> : std::true_type {};

The :cpp:`is_array` type trait can be used as follow:

.. code:: c++

  template<
    class Array,
    std::enable_if_t< is_array<Array> , int > =0
  > constexpr auto
  my_array_fun(const Array& x) {
    // ...
  }

The :cpp:`my_array_fun` function is constrained to accept only types for which the type trait :cpp:`is_array` is true.
