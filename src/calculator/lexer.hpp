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
        static const std::vector<std::pair<std::regex, TokenType>> tokenSpec;
        const char *curr_ptr_;

    public:
        Lexer(const char *cstr);

        std::optional<Token> next();
    };

} // namespace Calc
