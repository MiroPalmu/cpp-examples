#include <print>
#include <type_traits>
#include <typeinfo>
#include <utility>

template<typename T>
auto inspector(T&& t) {
    std::println("{} is const {}", typeid(T).name(), std::is_const_v<T>);
    std::println("{} is lvalue reference {}", typeid(T).name(), std::is_lvalue_reference_v<T>);
    std::println("{} is rvalue reference {}", typeid(T).name(), std::is_rvalue_reference_v<T>);
}

template<typename T>
auto not_so_perfect_inspector(T&& t) {
    std::println("Not so perfect:");
    inspector(t);
}

template<typename T>
auto perfect_inspector(T&& t) {
    std::println("Perfect:");
    inspector(std::forward<T>(t));
}

int main() {
    int a       = 1;
    const int b = 2;

    int& c       = a;
    const int& d = a;

    //int& e = b; // int& -> const int discards qualifiers
    const int& f = b;

    // int&& g = a; // int&& -> int cannot bind rvalue reference to lvalue
    // const int&& h = a; // const int&& -> int -||-

    // int&& i = b; // int&& -> const int -||-
    // const int&& j = b; // const int&& -> const int  -||-

    std::println("{}:", 1);
    inspector(1);
    not_so_perfect_inspector(1);
    perfect_inspector(1);
    std::println("{}:", "int a = 1");
    inspector(a);
    not_so_perfect_inspector(a);
    perfect_inspector(a);
    std::println("{}:", "const int b = 2");
    inspector(b);
    not_so_perfect_inspector(b);
    perfect_inspector(b);
    std::println("{}:", "int& c = a");
    inspector(c);
    not_so_perfect_inspector(c);
    perfect_inspector(c);
    std::println("{}:", "const int& d = a");
    inspector(d);
    not_so_perfect_inspector(d);
    perfect_inspector(d);
    std::println("{}:", "const int& f = b");
    inspector(f);
    not_so_perfect_inspector(f);
    perfect_inspector(f);
}
