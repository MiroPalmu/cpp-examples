#include <print>

template<typename D>
struct Base {
    int x;
    ~Base() {
        std::println("~Base x = {}", x);

        auto* as_D = static_cast<D*>(this);
        std::println("~Base y = {}", as_D->y);
    }
};

struct Derived : Base<Derived> {
    int y;
    ~Derived() { std::println("~Derived y = {}", y); }
};

int main() {
    // Because the destruction sequence is:
    //
    // 1) execute dtor body
    // 2) call dtor of all non-static non-variant data members, in reverse order of decleration
    // 3) call dtor of all idrect non-virtual base classes in reverse order of construction
    // etc.
    //
    // This means that the call to dtor of Derived is UB.
    // Lifetime of Derived::y ends at step 2) of Derived::~Derived and
    // is accessed in step 3) from Base::~Base.
    auto _ = Derived{ { 42 }, 100 };
}
