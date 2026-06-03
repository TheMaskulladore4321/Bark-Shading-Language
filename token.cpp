#include "token.h"

std::string TokenName(TokenType t)
{
    switch (t)
    {
        case TokenType::Uniform: return "Uniform";
        case TokenType::Input: return "Input";
        case TokenType::Output: return "Output";
        case TokenType::Varying: return "Varying";

        case TokenType::Vertex: return "Vertex";
        case TokenType::Fragment: return "Fragment";

        case TokenType::Float: return "Float";
        case TokenType::Int: return "Int";
        case TokenType::Bool: return "Bool";

        case TokenType::Vec2: return "Vec2";
        case TokenType::Vec3: return "Vec3";
        case TokenType::Vec4: return "Vec4";

        case TokenType::Identifier: return "Identifier";
        case TokenType::Number: return "Number";

        case TokenType::LParen: return "LParen";
        case TokenType::RParen: return "RParen";

        case TokenType::LBrace: return "LBrace";
        case TokenType::RBrace: return "RBrace";

        case TokenType::Comma: return "Comma";
        case TokenType::Dot: return "Dot";

        case TokenType::Equal: return "Equal";

        case TokenType::Plus: return "Plus";
        case TokenType::Minus: return "Minus";
        case TokenType::Star: return "Star";
        case TokenType::Slash: return "Slash";

        case TokenType::EndOfFile: return "EOF";

        default: return "Unknown";
    }
}

TokenType GetTokenType(const std::string& token)
{
    if (token == "uniform") return TokenType::Uniform;
    if (token == "input") return TokenType::Input;
    if (token == "output") return TokenType::Output;
    if (token == "varying") return TokenType::Varying;

    if (token == "vertex") return TokenType::Vertex;
    if (token == "fragment") return TokenType::Fragment;

    if (token == "float") return TokenType::Float;
    if (token == "int") return TokenType::Int;
    if (token == "bool") return TokenType::Bool;

    if (token == "vec2") return TokenType::Vec2;
    if (token == "vec3") return TokenType::Vec3;
    if (token == "vec4") return TokenType::Vec4;

    return TokenType::Identifier;
}