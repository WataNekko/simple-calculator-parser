#pragma once

#include <ostream>
#include <string_view>

namespace Calc {

    enum class TokenType : char {
        Ignore = 0,
        NUMBER = 'N',
        ADD = '+',
        SUB = '-',
        MULT = '*',
        DIV = '/',
        LPAREN = '(',
        RPAREN = ')',
        ANS = '_'
    };

    struct Token {
        TokenType type;
        std::string_view value;

        Token(TokenType type, const std::string_view &value);
        friend std::ostream &operator<<(std::ostream &os, const Token &token);
    };

} // namespace Calc
