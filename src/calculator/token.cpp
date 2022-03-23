#include "token.hpp"

namespace Calc {

    Token::Token(Token::Type type, const std::string_view &value)
        : type(type), value(value) {}

    std::ostream &operator<<(std::ostream &os, const Token &token)
    {
        os << "{ \"type\": \"" << static_cast<char>(token.type) << "\", \"value\": \"" << token.value << "\" }";
        return os;
    }

} // namespace Calc
