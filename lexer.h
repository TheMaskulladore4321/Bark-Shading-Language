#pragma once

#include <vector>
#include <string_view>

#include "token.h"

class Tokenizer
{
private:
    std::string_view src;
    size_t pos = 0;

    bool isAtEnd() const;
    char peek() const;
    char advance();
    bool isAlpha(char c);
    bool isAlphaNumeric(char c);

public:
    Tokenizer(std::string_view source);
    std::vector<Token> Tokenize();
};