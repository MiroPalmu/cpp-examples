#include <print>
#include <tuple>

template<template<typename> typename>
struct case_TT {};

template<template<typename> typename, typename>
struct case_TT_T {};

template<typename>
struct case_TT_T_special;

template<template<typename...> typename TT, typename... T>
struct case_TT_T_special<TT<T...>> {};

int main() {
    case_TT<std::tuple> _;
    // case_TT<std::tuple<int>> _;

    case_TT_T<std::tuple, int> _;
    // case_TT_T<std::tuple<int>> _;
    // case_TT_T<std::tuple<int>, int> _;

    // case_TT_T_special<std::tuple, int> _;
    case_TT_T_special<std::tuple<int>> _;
    // case_TT_T_special<std::tuple<int>, int> _;
    case_TT_T_special<std::tuple<int, float>> _;

    return 0;
}
