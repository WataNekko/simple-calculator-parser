#include "simple_calculator.hpp"

namespace Calc {

    AbstractSyntaxTree SimpleCalculator::parse(std::string expr)
    {
        return AbstractSyntaxTree(std::move(expr));
    }

    double SimpleCalculator::evaluate(const AbstractSyntaxTree &ast)
    {
        ans_ = ast.evaluate(ans_);
        return ans_;
    }

    double SimpleCalculator::evaluate(std::string expr)
    {
        auto ast = parse(std::move(expr));
        return evaluate(ast);
    }

} // namespace Calc
