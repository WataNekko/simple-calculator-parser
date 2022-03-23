#pragma once

#include <optional>
#include <regex>
#include <utility>
#include <vector>

#include "token.hpp"

namespace Calc {

    /**
     * Tokenizer. Convert a sequence of characters into tokens.
     */
    class Lexer {
    private:
        static const std::vector<std::pair<std::regex, Token::Type>> tokenSpec;
        const char *curr_ptr_;
        std::vector<Token> buffer_;

    public:
        Lexer(const Lexer &) = delete;
        Lexer(Lexer &&) = delete;
        Lexer(const char *cstr);

        Lexer &operator=(const Lexer &) = delete;
        Lexer &operator=(Lexer &&) = delete;

        // Get next token. Return no value if end of string is met.
        std::optional<Token> next();

        // putback a token into the buffer to be retrieved in a later next() call
        void putback(const Token &token);
    };

} // namespace Calc
