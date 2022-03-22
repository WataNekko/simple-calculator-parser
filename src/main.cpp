#include <iostream>

#include "calculator/abstract_syntax_tree.hpp"

int main()
{
    Calc::AbstractSyntaxTree ast(
        std::make_unique<Calc::AbstractSyntaxTree::UnOp>(
            '-',
            std::make_unique<Calc::AbstractSyntaxTree::UnOp>(
                '-',
                std::make_unique<Calc::AbstractSyntaxTree::BinOp>(
                    '+',
                    std::make_unique<Calc::AbstractSyntaxTree::BinOp>('-', nullptr, nullptr),
                    std::make_unique<Calc::AbstractSyntaxTree::Number>(12)))));
    std::cout << ast << '\n';
}
