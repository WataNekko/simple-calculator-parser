#include <iostream>

#include "calculator/lexer.hpp"

int main()
{
    Calc::Lexer lexer("1 + 2 - 1.2e-2 / 23");
    std::optional<Calc::Token> token;
    while (token = lexer.next()) {
        std::cout << *token << ",\n";
    }
}
