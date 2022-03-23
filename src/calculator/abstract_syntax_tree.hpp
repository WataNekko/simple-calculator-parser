#pragma once

#include <memory>
#include <ostream>
#include <string>

#include "lexer.hpp"

namespace Calc {

    /**
     * An abstract syntax tree created by parsing an expression with the following grammar.
     *
     * GRAMMAR:
     *
     * Expression:
     *     Term
     *     Expression '+' Term
     *     Expression '-' Term
     * Term:
     *     Primary
     *     Term '*' Primary
     *     Term '/' Primary
     * Primary:
     *     Number
     *     Ans
     *     '+' Primary
     *     '-' Primary
     *     '(' Expression ')'
     *     Primary '(' Expression ')'
     * Number:
     *     floating-point-literal
     * Ans:
     *     previously evaluated value
     */
    class AbstractSyntaxTree {
    public:
        class Node {
        public:
            // For printing nodes

            struct FormattedLF {
                const unsigned int indent_level;
                friend std::ostream &operator<<(std::ostream &os, const FormattedLF &lf);
            };

            class FormatNode {
            private:
                const std::unique_ptr<Node> &node_;

            public:
                const FormattedLF lf;

                FormatNode(const std::unique_ptr<Node> &node, const unsigned int indent_level = 0);
                FormatNode operator()(const std::unique_ptr<Node> &node) const;
                friend std::ostream &operator<<(std::ostream &os, const FormatNode &fmt);

            }; // class FormatNode

            virtual double evaluate() const = 0;
            virtual std::ostream &toJson(std::ostream &os, const FormatNode &fmt) const = 0;

        }; // class Node

        class Number : public Node {
        public:
            std::string_view value;

            Number(const std::string_view &value);

            double evaluate() const;
            std::ostream &toJson(std::ostream &os, const FormatNode &fmt) const;

        }; // class Number

        class Ans : public Node {
        public:
            double evaluate() const;
            std::ostream &toJson(std::ostream &os, const FormatNode &fmt) const;
        }; // class Ans

        class BinOp : public Node {
        public:
            enum Type : char {
                ADD = '+',
                SUB = '-',
                MULT = '*',
                DIV = '/'
            };

            Type type;
            std::unique_ptr<Node> left, right;

            BinOp(Type type, std::unique_ptr<Node> &&left, std::unique_ptr<Node> &&right);
            double evaluate() const;
            std::ostream &toJson(std::ostream &os, const FormatNode &fmt) const;

        }; // class BinOp

        class UnOp : public Node {
        public:
            enum Type : char {
                UPLUS = '+',
                UNEG = '-'
            };

            Type type;
            std::unique_ptr<Node> operand;

            UnOp(Type type, std::unique_ptr<Node> &&operand);
            double evaluate() const;
            std::ostream &toJson(std::ostream &os, const FormatNode &fmt) const;

        }; // class UnOp

        friend std::ostream &operator<<(std::ostream &os, const AbstractSyntaxTree &ast);

        AbstractSyntaxTree(std::string expr);

    private:
        std::string expr_;
        Lexer lexer_;
        std::unique_ptr<Node> root_;

        std::unique_ptr<Node> expression();
        std::unique_ptr<Node> term();
        std::unique_ptr<Node> primary();

    }; // class AbstractSyntaxTree

} // namespace Calc
