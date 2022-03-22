#include "abstract_syntax_tree.hpp"

namespace Calc {

    using Node = AbstractSyntaxTree::Node;
    using FormatNode = Node::FormatNode;
    using Number = AbstractSyntaxTree::Number;
    using BinOp = AbstractSyntaxTree::BinOp;
    using UnOp = AbstractSyntaxTree::UnOp;

    //============== AST ==============//

    std::ostream &operator<<(std::ostream &os, const AbstractSyntaxTree &ast)
    {
        return os << "{\n"
                  << "  \"root\": " << FormatNode(ast.root_, 1)
                  << "\n}";
    }

    //============== FormatNode ==============//

    FormatNode::FormatNode(const std::unique_ptr<Node> &node, const unsigned int indent_level)
        : node_(node), lf{indent_level} {}

    std::ostream &operator<<(std::ostream &os, const FormatNode &fmt)
    {
        if (fmt.node_) {
            return fmt.node_->toJson(os, fmt);
        }
        return os << "null";
    }

    // Used to format a child node (automatically increase the indent level)
    FormatNode FormatNode::operator()(const std::unique_ptr<Node> &node) const
    {
        return {node, lf.indent_level + 1};
    }

    // Insert new line and appropriate indentation
    std::ostream &operator<<(std::ostream &os, const Node::FormattedLF &lf)
    {
        os << '\n';
        if (lf.indent_level > 0) {
            os.width(2 * lf.indent_level);
            os << ' ';
        }
        return os;
    }

    //============== Number ==============//

    Number::Number(double value) : value(value) {}

    inline double Number::evaluate() const
    {
        return value;
    }

    std::ostream &Number::toJson(std::ostream &os, const FormatNode &fmt) const
    {
        return os << value;
    }

    //============== BinOp ==============//

    inline double BinOp::evaluate() const
    {
        return 0;
    }

    std::ostream &BinOp::toJson(std::ostream &os, const FormatNode &fmt) const
    {
        return os << '{' << fmt.lf
                  << "  \"type\": \"b" << static_cast<char>(type) << "\"," << fmt.lf
                  << "  \"left\": " << fmt(left) << ',' << fmt.lf
                  << "  \"right\": " << fmt(right) << fmt.lf
                  << '}';
    }

    //============== UnOp ==============//

    inline double UnOp::evaluate() const
    {
        return 0;
    }

    std::ostream &UnOp::toJson(std::ostream &os, const FormatNode &fmt) const
    {
        return os << '{' << fmt.lf
                  << "  \"type\": \"u" << static_cast<char>(type) << "\"," << fmt.lf
                  << "  \"operand\": " << fmt(operand) << fmt.lf
                  << '}';
    }

} // namespace Calc
