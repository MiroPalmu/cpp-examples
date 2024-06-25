#include <print>

class Foo {
    struct Bar {
        void public_member() {
            std::println("Hello from public member function of Bar!");
        }
    };

public:

    Bar get_bar() { return {}; }
};

int main() {
    auto foo = Foo{};
    auto bar = foo.get_bar();
    bar.public_member();
}
