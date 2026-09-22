#pragma once

// REF https://towardsdev.com/recursive-macros-in-c-20-the-dark-art-that-actually-works-f982c0f433e5
// There is a detailed explanation of what is going on
#define STD_E_PARENS ()

// We can't recurse until there is no argument, so here we generate 256 calls
#define STD_E_EXPAND_(...)  STD_E_EXPAND1_(STD_E_EXPAND1_(STD_E_EXPAND1_(STD_E_EXPAND1_(__VA_ARGS__))))
#define STD_E_EXPAND1_(...) STD_E_EXPAND2_(STD_E_EXPAND2_(STD_E_EXPAND2_(STD_E_EXPAND2_(__VA_ARGS__))))
#define STD_E_EXPAND2_(...) STD_E_EXPAND3_(STD_E_EXPAND3_(STD_E_EXPAND3_(STD_E_EXPAND3_(__VA_ARGS__))))
#define STD_E_EXPAND3_(...) STD_E_EXPAND4_(STD_E_EXPAND4_(STD_E_EXPAND4_(STD_E_EXPAND4_(__VA_ARGS__))))
#define STD_E_EXPAND4_(...) __VA_ARGS__

// FOR_EACH
#define STD_E_FOR_EACH(action, ...) \
    __VA_OPT__(STD_E_EXPAND_(STD_E_FOR_EACH_HELPER_(action, __VA_ARGS__)))

#define STD_E_FOR_EACH_HELPER_(action, first, ...) \
    action(first)                            \
    __VA_OPT__(STD_E_FOR_EACH_AGAIN_ STD_E_PARENS (action, __VA_ARGS__))

#define STD_E_FOR_EACH_AGAIN_() STD_E_FOR_EACH_HELPER_

// FOR_EACH_BIND1
#define STD_E_FOR_EACH_BIND1(action, bound1, ...) \
    __VA_OPT__(STD_E_EXPAND_(STD_E_FOR_EACH_BIND1_HELPER_(action, bound1, __VA_ARGS__)))

#define STD_E_FOR_EACH_BIND1_HELPER_(action, bound1, first, ...) \
    action(bound1, first)                            \
    __VA_OPT__(STD_E_FOR_EACH_BIND1_AGAIN_ STD_E_PARENS (action, bound1, __VA_ARGS__))

#define STD_E_FOR_EACH_BIND1_AGAIN_() STD_E_FOR_EACH_BIND1_HELPER_
