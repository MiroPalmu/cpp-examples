/// @file Compiler explorer link at bottom!

#include <type_traits>
#include <utility>

/// "Invoking" template TT using pack T..., i.e. TT<T...>, is valid type.
template<template<typename...> typename TT, typename... T>
concept template_invocable = requires { typename TT<T...>; };

///////////////
// # Case A: //
///////////////

#include <vector>

static_assert(template_invocable<std::vector, int>);
static_assert(template_invocable<std::vector, int, int>); // allocator is not constrained
static_assert(template_invocable<std::vector, int, std::allocator<int>>);

static_assert(not template_invocable<std::vector>);
static_assert(not template_invocable<std::vector, int, int, int>);

///////////////
// # Case B: //
///////////////

#include <ranges>
using namespace std::ranges;
// empty_view<T> has std::is_object_v<T> constraint.

static_assert(std::is_object_v<int>);
static_assert(template_invocable<empty_view, int>);

static_assert(not std::is_object_v<void>);
static_assert(not template_invocable<empty_view, void>);

///////////////
// # Case C: //
///////////////

static_assert(template_invocable<std::make_signed, unsigned>);
static_assert(template_invocable<std::make_signed_t, unsigned>);

////////////
// Case D //
////////////

// I would assume these to fail, but they do not.
struct Foo{ };
static_assert(template_invocable<std::make_signed, Foo>);
static_assert(template_invocable<std::make_signed, std::vector<int>>);

////////////
// Case E //
////////////

// I would assume these to fail, but they do not even compile, except on Clang with libc++:
// static_assert(not template_invocable<std::make_signed_t, Foo>);
// static_assert(not template_invocable<std::make_signed_t, std::vector<int>>)

/*
https://godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAMzwBtMA7AQwFtMQByARg9KtQYEAysib0QXACx8BBAKoBnTAAUAHpwAMvAFYTStJg1DIApACYAQuYukl9ZATwDKjdAGFUtAK4sGIAOwAnKSuADJ4DJgAcj4ARpjEIACs/qQADqgKhE4MHt6%2BehlZjgLhkTEs8Ylctpj2JQxCBEzEBHk%2BfkG19TlNLQRl0XEJyakKza3tBTXj/YMVVRIAlLaoXsTI7BzmAMwRyN5YANQmO24EAJ5pmAD6BMRMhAqn2CYaAIK7%2B4eYJ2dejlohAuLzenw%2BAHooSczGYAJIMABuqAA1hFgOYzEcCJgWGkDDijgAVIlHLxZIxHNJMZAo4kAOkZpCOeHpmHpxKJpzcRMZ9JezLwCiOiLEeHQ2Ku7LBOLxBMw3Nl%2BKYOMVUuYbD5L0l1w1vxJzMuutY7MZxNBHzQDE2aQI2NxypxNwiyNEsXofwAIkdiJgAI5ePC%2B4UmfwWHWME2c7m8xmgnbh0Oe05WD5gqEZzNZiHpiEwnZHNxMJRHd4gI5Q3PZ6tg2tmPbWn5/NyIzAOEgW8HvWaOZA3YtKVoQJXy51I1Bu%2Bjc8boEAgVvtsgswQvJYpsE9vB9gcJAjDh2jl0TpjuhVnGdzhdEJcRAiClc7bBrhMVvNiWjH68s4UMVB2q3jA8ESYOgG7NL2/YKIOe4jiqtxHpOZ5uBe85tte953kcKHvp%2BHZnLe8ZPuuaYfJu25QbuEC/nasFOghJ5TueBCzqhi6rsR3bgVukHQVRf72nKcFjq6DFIShV4kBhUnLgQ7EJrWkLVjWin5oWxa/BY5aVopSlZgpnz1t8XjHNyDxGJgzyPmC5LokceoKNSmxYcxc5mcAFkcdCDqXDciJ4JgADuMbaggxbOSxQo3KgsTaGhvnBY%2BRwAfcjyCPyJGcSq3E7kOKGRdFsUOPF%2BEPkR8mkVx5G8bR8Hjoh3LeRcvn%2BQFGFyamXZkTxlHUeFc75TFcWItyyLiu1YFZVVPX8TVwnHqeDV4j5fmBcyo2gY%2Bz4dVWukZrmqlFiWbhaTmOm7dpnWVd1Q6zfRC1MSxLBMCitxZMAkToMyXgMG9H3jRVk3XTBB5CXdjHIS5IBPS9Ny/SBdxfT9eDvSB/1dudp3vNCh2/N6F0Y/p0JwkcAVrLQEoDj4vwEAgFnU6gRxUI8tDMrEALYrTFxHOgDPUelnHEF4DhHAAYqgqChom/jJuVmUQTlwOCXRdWidOkPQ69yMfcyYsS5tHFdQr%2B5K7VIn3RDj3PZrKOfX1rHXtyBFWfrss7Up%2B040c2Cvm7yno3mxOk145NHJTbAc3T2IM0zdCs%2BzNOYFzPN2fxmCtgwSWoHidCYMymCqDadoCIWBiUgFhAIEcQKxKYljWCA%2B2GxRQ69bdKvmyhGuw1r8OYbraPQk3vGtyDytm%2BDndW93NsI3bEnEI7pXtRwKy0JwSS8H43C8KgnBuNY1hYWsGy/LsPCkAQmgrysKIgAAbFw9JJGYGgaJI/gABz%2BEkz87HfkhJH0JwSQm8r6kF3hwXgCgQAaAvlfFYcBYAwEQCgLOaQc5kAoBANA2d6CJGAAoZgaQFAID/HwOgOJiDQIgLEMBsQIgtAuJwc%2B9DmDEAuAAeUGg4ZhvAcFsEEBwhgtAmEcC0KQLAbNgBFloLQaB28JG4kMMAcQYjeD4F9A4PArZ5HiPzm2AEWxz63jqGA6uDx2EeCwGA%2B4eAWC8NIK2YgsRMiYE9EoowQIjDwL4AYAhAA1FqHDdQOP4IIEQYh2BSBkIIRQKh1BqNILoGopdjAH0sPoPAsRoGQBWKgW0OR5EAFoZynE9LXKwlgzB3x3k44g4oLLwBWHYNCOQXAMHcJ4DoegwjAQWCMGoRRsgCCmH4QZmRhkMHmMMao3RWkCD6JMLp0w5laIWRMAYfSZl6FmEs/IYzbAbOmZUAZzTj6bGWEAjgG9SBb3ERAo4qgP53yKf/I4wBkDICOMOQWDAURLG%2BbgQgJB8xcCWLwS%2BaiEFIDWAQNIAJyCUBwegvBUQTScCeS8t5Hyvk/O%2Bv83gIFgX1NnDUMJwhRDiC4GS2QcS1BgOSaQAKDw0i8NXuvUBiSIEcIBPCouVBHnPNeZId5nzvn3HxQCiAHhcEJFBeCuBUKb4gB2DsekqqNWaq1XfK5IDSD2NhPSb%2B/gdj%2BAfjsDQZh/AaGeakO5O9OBQJgYqrQCDkEQCQMijBiLsFoIwSgVJdxfkonIbQSh1DaGJNYYwhx0b2FcMKpC8%2B/DGAECESIsBkivDSPfPI8%2BWAnqeK2OIjRrSdFgP0cgQxDiTFr0SeYxhVji0QvqfYhRTiXFKHcYWlR6IfFUD8QoQJgVgmMFCbICJ4gzCAPJXShJ4jdBmH0MolA6SbDVxyRAPJBSBDFNKTscpa7qlHBKcxauZTq4VOsLUhIJLGm5NWQ0dpnT9k9I6ccxY4zig5FGYUCZDQP0DMfb0DZv6Zh1HmY0I5WyTmzN2W0ZZBz4OAeqGc9YFywVXJufa8BnBBVYpFQcZR4rg1SqBV%2BM%2BCrIWutIB6kAsK%2BU%2Bq9ai9FHBMXCqSqkkjkrCX4GvOKPQ5LJ1RJpbEpQ9LEmLqZSytlWHOX3M4DyuF7NUACvY28ojlI8V/KlTKlFcrdhmCo/AlYtMmBYESFu3VvADUf3pP/SQH9HOSAtVIO%2B/gPO3LARAp1sDqMr1o%2B6z1fq8FMZCyMTTwAg34tDeGygkbxFxtESwhh8buFJr4VnARabhGiJLZgKRMi5EOILco1RJagxlsaYkyt1aFG1rMVkixFwm02NbQ4jtrju1lb7VC3xTAAlBJCQooTlL2DTpifIcT86dAgGkKk1ddcMkbqaeAndP1OCno2oepbFgX43uIHezdzSINrL8BAVwYGQjvpg5%2B9I/6f2Ib/d%2B0ot2gMtLO4shDr7wM9HWXMN7cHQNPZmNB8o2zMMKHOVE9l1z5MOo4PhjjkXuM6cBXxkFlGIU%2BLowxhFWDmMJDRWwDFQqNNce0wSkIGOSWCYnWNkAE3Z3TYZXN6TTBWXb1h9h7zineUqbU2TwjFOJVo%2BleF4g%2BZJDGaVaQMzFnKCw71QatV/8zBcAfpayQd8H530CDsLzXLHW2Gdf5t18A6OE8wUiiX85kBpDSL5LggQbg7A/jcVQ/9YsJAjXQ1LyXeBJYTWhBxKbBG5czQV7NRW83qI8So5tEjKtaPLTV1QBicQ1sEKY%2BtTXG0YET7Ytt59Otdvj140AfWB0DaHUNsdI36eRMZzO2lLPJMquXd4q9y2snHbWw0eREILzd4sPKcYB2jurY%2B0%2Bi7HSru9PB7B57kyrtDIA4DnZp2Ghfau9PkDAPF93fg7vsHQwl%2BQ%2Bh5cutPOjeI89yKlgChERfMRM79VH90fEvldjvruP%2Bd3gJwl2Jy2ExSOEf2fxFDfzHwIF42JQE1EwpSby4CXWZ3iVZwN2ZQ51k2v3h1ww4CUz5SOFU0eTeXAJfzfzd2%2BT0wwXzB2Bl1dVM0wHMxGCszrWV0Z0kHpECB/gfkkEkCkECBfnc0NwU0gRNz8xM1IFvjd3pHfkCG4PVzMD4LNR2B/iuR2FwJ8xdWviuTME0ONzNxWCcSyGcEkCAA
*/
