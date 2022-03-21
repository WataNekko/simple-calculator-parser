#pragma once

#include <string>

#include <abstract_syntax_tree.hpp>

namespace Calc {

    class SimpleCalculator {
    private:
        double ans_;

    public:
        static AbstractSyntaxTree parse(const std::string &str);

        static double evaluate(const AbstractSyntaxTree &ast);

        static double evaluate(const std::string &str);

        double evaluate(const AbstractSyntaxTree &ast);

        double evaluate(const std::string &str);

        double ans();
    };

}
