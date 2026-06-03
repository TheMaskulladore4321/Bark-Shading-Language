#pragma once
#include <string>


enum class TokenType
{
    Uniform,
    Input,
    Output,
    Varying,

    Vertex,
    Fragment,

    Float,
    Int,
    Bool,

    Vec2,
    Vec3,
    Vec4,

    Identifier,
    Number,

    LParen,
    RParen,

    LBrace,
    RBrace,

    Comma,
    Dot,

    Equal,

    Plus,
    Minus,
    Star,
    Slash,

    EndOfFile,

    Unknown
};

class Token
{
public:
    TokenType type;
    std::string lexeme;

    Token(TokenType t, const std::string& l)
        : type(t), lexeme(l)
    {}
};

TokenType GetTokenType(const std::string& token);
std::string TokenName(TokenType type);