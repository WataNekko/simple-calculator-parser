#include "lexer.hpp"

namespace Calc {

    const std::vector<std::pair<std::regex, TokenType>> Lexer::tokenSpec = {
        {std::regex(R"(\s+)"), TokenType::Ignore},
        {std::regex(R"(\+)"), TokenType::ADD},
        {std::regex(R"(-)"), TokenType::SUB},
        {std::regex(R"(\*)"), TokenType::MULT},
        {std::regex(R"(\/)"), TokenType::DIV},
        {std::regex(R"(\()"), TokenType::LPAREN},
        {std::regex(R"(\))"), TokenType::RPAREN},
        {std::regex(R"(ans)"), TokenType::ANS},
        {std::regex(R"(((\d*\.\d+)|(\d+\.?))([eE][-+]?\d+)?(?![eE\d\.]))"), TokenType::NUMBER}};

    Lexer::Lexer(const char *cstr) : curr_ptr_(cstr) {}

    std::optional<Token> Lexer::next()
    {
        if (*curr_ptr_ == '\0') {
            return {}; // end of string
        }

        std::cmatch match;
        for (auto [re, type] : tokenSpec) {
            if (std::regex_search(curr_ptr_, match, re,
                                  std::regex_constants::match_continuous)) {

                if (type == TokenType::Ignore) {
                    curr_ptr_ += match.length();
                    return next();
                }

                std::string_view value(curr_ptr_, match.length());
                curr_ptr_ += match.length();
                return Token(type, value);
            }
        }

        throw "Error: Invalid syntax"; // no defined spec match
    }

}