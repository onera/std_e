Serialization / deserialization
===============================

Three issues make serialization/deserialization difficult to implement :

* unsafe type operations (casts from/to untyped memory, alignement...)
* lack of reflexion in the language
* designing efficient, general, easy-to-use interfaces

Let us focus on the last point. One of the main problem is that we don't want to do work that is not needed. In particular, we want to
* avoid copy from/to contiguous untyped memory if what we are serializing/deserializing is already contiguous and just needs to be seen as typed
* avoid memory allocations when the object are already created (in particular, avoid "deallocate an array, then allocate to replace it by an array of the same size)

This is why they are a bunch of related functions regarding serialization/deserialization, each with its own use. This also why the function interfaces are not uniform: depending on the type of data to serialize or deserialize, the work to be done may be slightly different.

We distiguish 3 kinds of types:
* `Trivially_copyable_value` types that are trivially copyable and that contain no pointer member. The `Trivially_copyable_value` concept is not implementable of now (with no reflexion) because of the "contain no pointer member" part.
* Contiguous arrays of `Trivially_copyable_value`.
* Other types.

Let us call the first two `Trivially_serializable`.

Remarks related to permormance
------------------------------

* Serialization and deserialization should not be considered as exact reverse operations. A serialized object may be deserialized in a object of another type. Common example: serializing a `span<int>` and deserializing it into a `vector<int>`.

* For non-\ `Trivially_serializable` types, the interfaces are simple since there must be a copy from/to serialized memory:
  * `serialize(const Non_trivially_serializable&) -> vector<byte>`
  * `deserialize(const std::byte* ptr, int n) -> Non_trivially_serializable`

* For serialization a `Trivially_serializable` object, the resulting array of bytes is just the memory of this very object, seen as serialized, untyped memory. this is why the interface is
  * `serialize(const Trivially_serializable&) -> `span<byte>` (the `span` is of fixed size in case of a `Trivially_copyable_value`, and of dynamic size in case of an array of `Trivially_copyable_value`).


* For deserialization a `Trivially_serializable` object, there are more possibilities.
  * the `deserialize(const std::byte* ptr, int n) -> Trivially_serializable` interface is still usable.
  * however, it means there will be a copy, and a memory allocation for arrays.
  * the copy could be avoided using `deserialize(Callable f, int n) -> Trivially_serializable` where `f` is the function that will retrive the serialized object. `f` is of prototype `f(std::byte* ptr, int size) -> void`. The idea is the `deserialize` will allocate the final object, pass its memory to `f`, let `f` fill this memory with the serialized object, then, since the oject is `Trivially_serializable`, there is actually nothing more to do, so `deserialize` can return the object.
  * if the object has already been allocated, `deserialize_into(Callable f, int n, Trivially_serializable& obj)` can be used instead.
  * finally for completeness, if we don't mind the copy but do not want to allocate, `deserialize_into(const std::byte *ptr , int n, Trivially_serializable& obj)` can be used instead.

Note
^^^^

For array deserialization, `deserialize_into` actually allocates memory if the distination array provided is not big enought. The key point is that it doesn't allocate if there is enought memory.
