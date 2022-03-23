#include <iostream>
#include <string>

#include "calculator/abstract_syntax_tree.hpp"

int main()
{
    std::string expr;
    while (std::getline(std::cin, expr)) {
        try {
            Calc::AbstractSyntaxTree ast(std::move(expr));
            std::cout << ast << '\n'
                      << std::endl;
        }
        catch (const char *ex) {
            std::cout << ex << '\n'
                      << std::endl;
        }
    }
}
