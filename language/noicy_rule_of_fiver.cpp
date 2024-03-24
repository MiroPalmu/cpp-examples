#include <cstring>
#include <iostream>
#include <string>
#include <utility>
class rule_of_five {
    char* cstring; // raw pointer used as a handle to a
                   // dynamically-allocated memory block
  public:
    explicit rule_of_five(const char* s = "") : cstring(nullptr) {
        std::cout << "Constructor\n";
        if (s) {
            std::size_t n = std::strlen(s) + 1;
            cstring       = new char[n]; // allocate
            std::memcpy(cstring, s, n);  // populate
        }
    }

    ~rule_of_five() { std::cout << "Destructor\n"; }

    rule_of_five(const rule_of_five& other) // copy constructor
        : cstring(other.cstring) {
        std::cout << "Copy constructor\n";
    }

    rule_of_five(rule_of_five&& other) noexcept // move constructor
        : cstring(std::exchange(other.cstring, nullptr)) {
        std::cout << "Move constructor\n";
    }

    rule_of_five& operator=(const rule_of_five& other) // copy assignment
    {
        std::cout << "Copy assignment\n";

        return *this = rule_of_five(other);
    }

    rule_of_five& operator=(rule_of_five&& other) noexcept // move assignment
    {
        std::cout << "Move assignment\n";

        std::swap(cstring, other.cstring);
        return *this;
    }
};

struct Foo {
    rule_of_five m_;
    Foo(const rule_of_five& m) : m_(m) {}
};

int main() {
    auto aa = rule_of_five("moi");

    auto p = Foo(aa);
}
