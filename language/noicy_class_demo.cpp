#include "noicy_class.hpp"

struct Foo {
    noicy_string_view m_;
    Foo(const noicy_string_view& m) : m_(m) {}
};

int main() {
    auto aa = noicy_string_view("moi");
    auto p  = Foo(aa);
}
