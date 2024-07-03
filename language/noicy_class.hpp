#pragma once

#include <print>
#include <string_view>

class noicy_string_view {
    std::string_view sv_;

  public:
    std::string_view sv(this auto&& self) { return self.sv_; }

    explicit noicy_string_view(const std::string_view sv) : sv_{ sv } {
        std::println("Constructor of {}.", sv_);
    }

    ~noicy_string_view() { std::println("Destructor of {}.", sv_); }

    noicy_string_view(const noicy_string_view& other) : sv_{ other.sv_ } {
        std::println("Copy constructor of {}.", sv_);
    }

    noicy_string_view(noicy_string_view&& other) noexcept : sv_{ other.sv_ } {
        std::println("Move constructor of {}.", sv_);
    }

    noicy_string_view& operator=(const noicy_string_view& other) {
        sv_ = other.sv_;
        std::println("Copy assignment of {}.", sv_);
        return *this;
    }

    noicy_string_view& operator=(noicy_string_view&& other) noexcept {
        sv_ = other.sv_;
        std::println("Move assignment of {}.", sv_);
        return *this;
    }
};
