module;
#include <print>
#include <utility>
export module sstd;

export namespace sstd {
template<typename T>
void
generic_print(T&& t) {
    std::println("This is generic print: {}", std::forward<T>(t));
}
} // namespace sstd
