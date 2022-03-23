#include <fstream>
#include <iostream>
#include <string>

#include "calculator/simple_calculator.hpp"

int main(int argc, const char *argv[])
{
    // validate arguments
    if (argc > 2) {
        std::cout << "Error: More than one argument specified\n";
        return 1;
    }

    Calc::SimpleCalculator calc;
    std::string expr;

    // main loop
    while (true) {
        std::cout << ">> ";
        std::getline(std::cin, expr);

        try {
            // parse and evaluate expression
            auto ast = calc.parse(expr);
            double ans = calc.evaluate(ast);

            std::cout << " = " << ans << '\n'
                      << std::endl;

            // output AST to file if specified file name argument
            if (argc == 2) {
                std::ofstream(argv[1]) << ast << std::endl;
            }
        }
        catch (const char *ex) {
            std::cout << ex << '\n'
                      << std::endl;
        }
    }
}
