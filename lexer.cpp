#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "token.h"
#include "lexer.h"

Tokenizer::Tokenizer(std::string_view source)
    : src(source)
{
}

bool Tokenizer::isAtEnd() const
{
    return pos >= src.length();
}

char Tokenizer::peek() const
{
    if (isAtEnd()) return '\0';
    return src[pos];
}

char Tokenizer::advance()
{
    if (isAtEnd()) return '\0';
    return src[pos++];
}

bool Tokenizer::isAlpha(char c)
{
    return std::isalpha(static_cast<unsigned char>(c)) || c == '_';
}

bool Tokenizer::isAlphaNumeric(char c)
{
    return std::isalnum(static_cast<unsigned char>(c)) || c == '_';
}

std::vector<Token> Tokenizer::Tokenize()
{
    std::vector<Token> tokens;

    while (!isAtEnd())
    {
        char c = peek();

        if (std::isspace(static_cast<unsigned char>(c)))
        {
            advance();
            continue;
        }

        switch (c)
        {
            case '(':
                tokens.emplace_back(TokenType::LParen, "(");
                advance();
                continue;

            case ')':
                tokens.emplace_back(TokenType::RParen, ")");
                advance();
                continue;

            case '{':
                tokens.emplace_back(TokenType::LBrace, "{");
                advance();
                continue;

            case '}':
                tokens.emplace_back(TokenType::RBrace, "}");
                advance();
                continue;

            case ',':
                tokens.emplace_back(TokenType::Comma, ",");
                advance();
                continue;

            case '.':
                tokens.emplace_back(TokenType::Dot, ".");
                advance();
                continue;

            case '=':
                tokens.emplace_back(TokenType::Equal, "=");
                advance();
                continue;

            case '+':
                tokens.emplace_back(TokenType::Plus, "+");
                advance();
                continue;

            case '-':
                tokens.emplace_back(TokenType::Minus, "-");
                advance();
                continue;

            case '*':
                tokens.emplace_back(TokenType::Star, "*");
                advance();
                continue;

            case '/':
                tokens.emplace_back(TokenType::Slash, "/");
                advance();
                continue;
        }

        if (isAlpha(c))
        {
            std::string word;

            while (!isAtEnd() && isAlphaNumeric(peek()))
            {
                word += advance();
            }

            tokens.emplace_back(GetTokenType(word), word);
            continue;
        }

        if (std::isdigit(static_cast<unsigned char>(c)))
        {
            std::string number;

            while (!isAtEnd() &&
                   (std::isdigit(static_cast<unsigned char>(peek())) ||
                    peek() == '.'))
            {
                number += advance();
            }

            tokens.emplace_back(TokenType::Number, number);
            continue;
        }

        tokens.emplace_back(
            TokenType::Unknown,
            std::string(1, advance())
        );
    }

    tokens.emplace_back(TokenType::EndOfFile, "");
    return tokens;
}
