#ifndef STD_E__LIFT_H
#define STD_E__LIFT_H


// Purpose: LIFT(f) converts a template function/set of overloaded functions named "f"
// into a generic lambda, callable with exactly the same arguments that would be possible for "f"



// from https://stackoverflow.com/a/45187436/1583122

// C++ requires you to type out the same function body three times to obtain
// SFINAE-friendliness and noexcept-correctness. That's unacceptable.
#if __GNUC__ > 7
#define RETURNS(...) noexcept(noexcept(__VA_ARGS__)) \
     -> decltype(__VA_ARGS__){ return __VA_ARGS__; }
#else
#define RETURNS(...) -> decltype(__VA_ARGS__){ return __VA_ARGS__; }
#endif


// The name of overload sets can be legally used as part of a function call -
// we can use a macro to create a lambda for us that "lifts" the overload set
// into a function object.
#define LIFT(f) [](auto&&... xs) RETURNS(f(::std::forward<decltype(xs)>(xs)...))


#endif
