.. _val_ref_view:

Value, reference and view types
===============================

The meaning of reference and view types can be a bit blurry at times. Here is an attempt to define them more precisely.

Value type
----------

We use the term **value type** as a synonym of `Regular <https://en.cppreference.com/w/cpp/concepts/regular>`_ type. That is, a type that "behaves like an :cpp:`int`": it can be reasoned about like an :cpp:`int`, in particular regarding construction, copy, assignement and comparison. (Note: a **Semi-regular** type can arguably also be called a value type).

Reference type
--------------

A **Reference** type is a type that behaves like an :cpp:`int&`. Semantically, a reference is always refering to an underlying object and every operation done to the reference is actually applied to the referred object.

A **Reference** is never **Regular** because its assignment semantics is always different from a regular type. Assigning to a reference assigns to its underlying object:

.. code:: c++

  int i=5, j=10;
  int& i_ref = i;
  i_ref = j; // now i == j, but i_ref did not change: it is still refering to i

Because the assignment operator is already used for assigning to the underlying object, references are not re-assignable (which mean that the underlying object cannot be replaced by another one). And because of this, they are not default constructible, because then there would be no mean to make them refer to a valid object later on.

Dereferencing an iterator must always return a reference.

Most of the time, references are actually **built-in references**. However, the need to create some other reference types sometimes arises. They are often called **proxy references** to distinguish them from built-in references. Proxy references have a pretty mediocre language support (no :cpp:`operator.`) and standard library support (:cpp:`Forward_iterator` requires that dereferencing returns a built-in reference); but that might change in the future.

.. note::
  :cpp:`std::reference_wrapper` is **not** a Reference, because its assignment operator is defined as rebinding to another object.

View type
---------

A **View** is a type that behaves like a :cpp:`std::span<int>`. A view over a :cpp:`T` is a collection of elements of type :cpp:`T`, where the **elements are not owned by the view**. Moreover, a view only really makes sense over a collection (an integer view seems like a not very useful feature): the memory is supposed to be owned by some kind of container object.

Maybe somewhat surprisingly, **views are Regular**: for all :cpp:`T x;`, we have the equivalence :cpp:`view<T> x_view(x); <=> view<T> x_view; x_view = x;`. That is why even if views and references may seem similar because they refer to external memory, their semantic is very different:

* A view is a Regular collection whose elements are owned by other objects. However, it can hold its own data; for instance, its own size (such as :cpp:`span`).
* A reference is refering to one object and delegating everything to it (including assignment). It has access to all the parts of the object, and it does not hold any data that would not be possessed by the object.

.. note::
  It seems that sometimes, "view" only means "immutable view". This is not the case here. By convention view types are often suffixed by :cpp:`_view` and non-modifying views are suffixed by :cpp:`_const_view`. :cpp:`span` and :cpp:`strided_span` are exceptions to this naming convention.

Performance
"""""""""""

Views are cheap to create and copy. They are often used to represent sub-collections. For example, a :cpp:`span` can be used to represent a contiguous sub-range of a :cpp:`std::vector`.

It does not mean they are free. For example, multi-dimensional array views copy the shape of their parent. This is a moderately expansive operation which even needs to allocate memory in case the shape's rank is dynamic.
