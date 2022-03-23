#pragma once

#include <ostream>
#include <string_view>

namespace Calc {

    struct Token {
        enum Type : char {
            Ignore = 0,
            NUMBER = 'N',
            PLUS = '+',
            MINUS = '-',
            MULT = '*',
            DIV = '/',
            LPAREN = '(',
            RPAREN = ')',
            ANS = '_'
        };

        Type type;
        std::string_view value;

        Token(Type type, const std::string_view &value);
        friend std::ostream &operator<<(std::ostream &os, const Token &token);
    };

} // namespace Calc
