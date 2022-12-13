//
// Created by msobral on 13/12/2022.
//
#include <iostream>
#include <string>
#include <optional>
#include <variant>

int main() {
    std::optional<int> op1{};
    std::variant<int,double> v;
//    op1.emplace(5);
//    op1.reset();

    if (op1.has_value()) {
        std::cout << "Valor=" << *op1 << std::endl;
    } else {
        decltype(op1)::value_type x{};
        std:: cout << "ou: " << x << std::endl;
    }

    v = 3.1;
    if (auto * p = std::get_if<int>(&v)) {
        std::cout << "int=" << *p << std::endl;
    } else if (auto * p = std::get_if<double>(&v)) {
        std::cout << "double=" << *p << std::endl;
    }
}