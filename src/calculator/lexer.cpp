#include "lexer.hpp"

namespace Calc {

    const std::vector<std::pair<std::regex, Token::Type>> Lexer::tokenSpec = {
        {std::regex(R"(\s+)"), Token::Ignore},
        {std::regex(R"(\+)"), Token::PLUS},
        {std::regex(R"(-)"), Token::MINUS},
        {std::regex(R"(\*)"), Token::MULT},
        {std::regex(R"(\/)"), Token::DIV},
        {std::regex(R"(\()"), Token::LPAREN},
        {std::regex(R"(\))"), Token::RPAREN},
        {std::regex(R"(ans)"), Token::ANS},
        {std::regex(R"(((\d*\.\d+)|(\d+\.?))([eE][-+]?\d+)?(?![eE\d\.]))"), Token::NUMBER}};

    Lexer::Lexer(const char *cstr) : curr_ptr_(cstr) {}

    std::optional<Token> Lexer::next()
    {
        if (!buffer_.empty()) {
            // available in buffer
            Token token = buffer_.back();
            buffer_.pop_back();
            return token;
        }

        if (*curr_ptr_ == '\0') {
            return {}; // end of string
        }

        std::cmatch match;
        for (const auto &[re, type] : tokenSpec) {
            if (std::regex_search(curr_ptr_, match, re,
                                  std::regex_constants::match_continuous)) {

                if (type == Token::Ignore) {
                    curr_ptr_ += match.length();
                    return next();
                }

                std::string_view value(curr_ptr_, match.length());
                curr_ptr_ += match.length();
                return Token(type, value);
            }
        }

        throw "SyntaxError: Unrecognized token"; // no defined spec match
    }

    void Lexer::putback(const Token &token)
    {
        buffer_.push_back(token);
    }

} // namespace Calc
