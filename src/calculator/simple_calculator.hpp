#pragma once

#include <string>

#include "abstract_syntax_tree.hpp"

namespace Calc {

    class SimpleCalculator {
    private:
        double ans_ = 0.0;

    public:
        static AbstractSyntaxTree parse(std::string expr);

        double evaluate(const AbstractSyntaxTree &ast);
        double evaluate(std::string expr);

        inline double ans() { return ans_; }
    };

}
