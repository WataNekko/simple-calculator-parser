#include <cstdlib>

#include "abstract_syntax_tree.hpp"

namespace Calc {

    using Node = AbstractSyntaxTree::Node;
    using FormatNode = Node::FormatNode;
    using Ans = AbstractSyntaxTree::Ans;
    using Number = AbstractSyntaxTree::Number;
    using BinOp = AbstractSyntaxTree::BinOp;
    using UnOp = AbstractSyntaxTree::UnOp;

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
        for (unsigned int i = 0; i < lf.indent_level; i++) {
            os << "  ";
        }
        return os;
    }

    //============== Node ==============//

    double Node::evaluate(const std::unique_ptr<Node> &node, double ans)
    {
        if (!node) {
            throw "TypeError: Cannot evaluate null node";
        }
        return node->evaluate(ans);
    }

    //============== Number ==============//

    Number::Number(const std::string_view &value) : value(value) {}

    double Number::evaluate(double ans) const
    {
        return std::atof(value.data());
    }

    std::ostream &Number::toJson(std::ostream &os, const FormatNode &fmt) const
    {
        return os << '{' << fmt.lf
                  << "  \"type\": \"number\"," << fmt.lf
                  << "  \"value\": \"" << value << '\"' << fmt.lf
                  << '}';
    }

    //============== Ans ==============//

    double Ans::evaluate(double ans) const
    {
        return ans;
    }

    std::ostream &Ans::toJson(std::ostream &os, const FormatNode &fmt) const
    {
        return os << '{' << fmt.lf
                  << "  \"type\": \"ans\"" << fmt.lf
                  << '}';
    }

    //============== BinOp ==============//

    BinOp::BinOp(BinOp::Type type, std::unique_ptr<Node> &&left, std::unique_ptr<Node> &&right)
        : type(type), left(std::move(left)), right(std::move(right)) {}

    double BinOp::evaluate(double ans) const
    {
        double a = Node::evaluate(left, ans);
        double b = Node::evaluate(right, ans);

        switch (type) {
        case ADD:
            return a + b;
        case SUB:
            return a - b;
        case MULT:
            return a * b;
        case DIV:
            return a / b;
        default:
            throw "TypeError: Unknown operator type";
        }
    }

    std::ostream &BinOp::toJson(std::ostream &os, const FormatNode &fmt) const
    {
        return os << '{' << fmt.lf
                  << "  \"type\": \"" << static_cast<char>(type) << "\"," << fmt.lf
                  << "  \"left\": " << fmt(left) << ',' << fmt.lf
                  << "  \"right\": " << fmt(right) << fmt.lf
                  << '}';
    }

    //============== UnOp ==============//

    UnOp::UnOp(UnOp::Type type, std::unique_ptr<Node> &&operand)
        : type(type), operand(std::move(operand)) {}

    double UnOp::evaluate(double ans) const
    {
        double val = Node::evaluate(operand, ans);

        switch (type) {
        case UPLUS:
            return val;
        case UNEG:
            return 0.0 - val; // instead of "return -val;" to avoid producing negative zero (-0.0)
        default:
            throw "TypeError: Unknown operator type";
        }
    }

    std::ostream &UnOp::toJson(std::ostream &os, const FormatNode &fmt) const
    {
        return os << '{' << fmt.lf
                  << "  \"type\": \"u" << static_cast<char>(type) << "\"," << fmt.lf
                  << "  \"operand\": " << fmt(operand) << fmt.lf
                  << '}';
    }

    //============== AST ==============//

    AbstractSyntaxTree::AbstractSyntaxTree(std::string expr)
        : expr_(std::move(expr)), lexer_(expr_.c_str()), root_(expression())
    {
        std::optional<Token> token = lexer_.next();
        if (token) {
            throw "SyntaxError: Unexpected token";
        }
    }

    double AbstractSyntaxTree::evaluate(double ans) const
    {
        return Node::evaluate(root_, ans);
    }

    std::ostream &operator<<(std::ostream &os, const AbstractSyntaxTree &ast)
    {
        return os << "{\n"
                  << "  \"expr\": \"" << ast.expr_ << "\",\n"
                  << "  \"root\": " << FormatNode(ast.root_, 1) << '\n'
                  << '}';
    }

    std::unique_ptr<Node> AbstractSyntaxTree::expression()
    {
        auto expr = term();

        std::optional<Token> token;
        while ((token = lexer_.next())) {

            switch (token->type) {
            case Token::PLUS:
                expr = std::make_unique<BinOp>(BinOp::ADD, std::move(expr), term());
                break;

            case Token::MINUS:
                expr = std::make_unique<BinOp>(BinOp::SUB, std::move(expr), term());
                break;

            default:
                lexer_.putback(*token);
                return expr;
            }
        }

        return expr;
    }

    std::unique_ptr<Node> AbstractSyntaxTree::term()
    {
        auto term = primary();

        std::optional<Token> token;
        while ((token = lexer_.next())) {

            switch (token->type) {
            case Token::MULT:
                term = std::make_unique<BinOp>(BinOp::MULT, std::move(term), primary());
                break;

            case Token::DIV:
                term = std::make_unique<BinOp>(BinOp::DIV, std::move(term), primary());
                break;

            default:
                lexer_.putback(*token);
                return term;
            }
        }

        return term;
    }

    std::unique_ptr<Node> AbstractSyntaxTree::primary()
    {
        std::optional<Token> token = lexer_.next();
        if (!token) {
            throw "SyntaxError: Unexpected end of line";
        }

        std::unique_ptr<Node> prim;
        switch (token->type) {
        case Token::NUMBER:
            prim = std::make_unique<Number>(token->value);
            break;

        case Token::ANS:
            prim = std::make_unique<Ans>();
            break;

        case Token::PLUS:
            prim = std::make_unique<UnOp>(UnOp::UPLUS, primary());
            break;

        case Token::MINUS:
            prim = std::make_unique<UnOp>(UnOp::UNEG, primary());
            break;

        case Token::LPAREN:
            prim = expression();

            if ((token = lexer_.next()) && token->type == Token::RPAREN) {
                break;
            }
            throw "SyntaxError: Expected closing parenthesis";

        default:
            throw "SyntaxError: Expected primary expression";
        }

        while ((token = lexer_.next())) {

            switch (token->type) {
            case Token::LPAREN:
                prim = std::make_unique<BinOp>(BinOp::MULT, std::move(prim), expression());

                if ((token = lexer_.next()) && token->type == Token::RPAREN) {
                    break;
                }
                throw "SyntaxError: Expected closing parenthesis";

            default:
                lexer_.putback(*token);
                return prim;
            }
        }

        return prim;
    }

} // namespace Calc
