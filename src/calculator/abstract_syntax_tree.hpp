#pragma once

#include <memory>
#include <ostream>

namespace Calc {

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
            double value;

            Number(double value);

            double evaluate() const;
            std::ostream &toJson(std::ostream &os, const FormatNode &fmt) const;

        }; // class Number

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

            double evaluate() const;
            std::ostream &toJson(std::ostream &os, const FormatNode &fmt) const;

            BinOp(char type, std::unique_ptr<Node> &&left, std::unique_ptr<Node> &&right)
                : type(static_cast<Type>(type)), left(std::move(left)), right(std::move(right)){};

        }; // class BinOp

        class UnOp : public Node {
        public:
            enum Type : char {
                UPLUS = '+',
                UNEG = '-'
            };

            Type type;
            std::unique_ptr<Node> operand;

            double evaluate() const;
            std::ostream &toJson(std::ostream &os, const FormatNode &fmt) const;

            UnOp(char type, std::unique_ptr<Node> &&node) : type(static_cast<Type>(type)), operand(std::move(node)){};

        }; // class UnOp

        friend std::ostream &operator<<(std::ostream &os, const AbstractSyntaxTree &ast);

        AbstractSyntaxTree(std::unique_ptr<Node> &&ptr) : root_(std::move(ptr)) {}

    private:
        std::unique_ptr<Node> root_;

    }; // class AbstractSyntaxTree

} // namespace Calc
