#include "parser.h"
#include <stdexcept>
#include <iostream>

Token& Parser::Peek()
{
    return tokens[current];
}

Token& Parser::Previous()
{
    return tokens[current - 1];
}

bool Parser::IsAtEnd()
{
    return Peek().type == TokenType::EndOfFile;
}

Token Parser::Advance()
{
    if (!IsAtEnd())
        current++;

    return Previous();
}

bool Parser::Check(TokenType type)
{
    if (IsAtEnd())
        return false;

    return Peek().type == type;
}

bool Parser::Match(TokenType type)
{
    if (!Check(type))
        return false;

    Advance();
    return true;
}

Token Parser::Consume(TokenType expected, const std::string& message)
{
    if (Check(expected))
        return Advance();

    throw std::runtime_error(message);
}

Parser::Parser(const std::vector<Token>& toks)
    : tokens(toks)
{
}

VariableDecl Parser::ParseDeclaration()
{
    VariableDecl decl;

    decl.qualifier =
        Advance().lexeme;

    decl.type =
        Advance().lexeme;

    decl.name =
        Consume(
            TokenType::Identifier,
            "Expected variable name"
        ).lexeme;

    return decl;
}

ShaderBlock Parser::ParseShaderBlock()
{
    ShaderBlock block;

    Advance();

    Consume(
        TokenType::LBrace,
        "Expected {"
    );

    while (
        !Check(TokenType::RBrace) &&
        !IsAtEnd()
    )
    {
        block.statements.push_back(
            ParseStatement()
        );
    }

    Consume(
        TokenType::RBrace,
        "Expected }"
    );

    return block;
}

Program Parser::Parse()
{
    Program program;

    while (!IsAtEnd())
    {
        if (Check(TokenType::Uniform) ||
            Check(TokenType::Input) ||
            Check(TokenType::Output) ||
            Check(TokenType::Varying))
        {
            program.globals.push_back(
                ParseDeclaration()
            );
        }
        else if (Check(TokenType::Vertex))
        {
            program.vertex =
                ParseShaderBlock();
        }
        else if (Check(TokenType::Fragment))
        {
            program.fragment =
                ParseShaderBlock();
        }
        else
        {
            throw std::runtime_error(
                "Unexpected token"
            );
        }
    }

    return program;
}

Expr* Parser::ParseExpression()
{
    return ParseAddition();
}

Expr* Parser::ParseAddition()
{
    Expr* expr =
        ParseMultiplication();

    while (
        Check(TokenType::Plus) ||
        Check(TokenType::Minus)
    )
    {
        Token op = Advance();

        Expr* right =
            ParseMultiplication();

        expr =
            new BinaryExpr(
                expr,
                op,
                right
            );
    }

    return expr;
}

Expr* Parser::ParseMultiplication()
{
    Expr* expr =
        ParseUnary();

    while (
        Check(TokenType::Star) ||
        Check(TokenType::Slash)
    )
    {
        Token op = Advance();

        Expr* right =
            ParseUnary();

        expr =
            new BinaryExpr(
                expr,
                op,
                right
            );
    }

    return expr;
}

Expr* Parser::ParsePrimary()
{
    if (Check(TokenType::Number))
    {
        return new LiteralExpr(
            Advance().lexeme
        );
    }

    if (
        Check(TokenType::Identifier) ||
        Check(TokenType::Vec2) ||
        Check(TokenType::Vec3) ||
        Check(TokenType::Vec4)
    )
    {
        std::string name =
            Advance().lexeme;

        if (Check(TokenType::LParen))
        {
            Advance();

            std::vector<Expr*> args;

            if (!Check(TokenType::RParen))
            {
                do
                {
                    args.push_back(
                        ParseExpression()
                    );

                    if (!Check(TokenType::Comma))
                        break;

                    Advance();

                } while (true);
            }

            Consume(
                TokenType::RParen,
                "Expected )"
            );

            return new CallExpr(
                name,
                args
            );
        }

        if (Check(TokenType::Dot))
        {
            Advance();

            std::string member =
                Consume(
                    TokenType::Identifier,
                    "Expected member"
                ).lexeme;

            return new MemberExpr(
                new VariableExpr(name),
                member
            );
        }

        return new VariableExpr(name);
    }

    if (Check(TokenType::LParen))
    {
        Advance();

        Expr* expr =
            ParseExpression();

        Consume(
            TokenType::RParen,
            "Expected )"
        );

        return expr;
    }

    throw std::runtime_error(
        "Expected expression"
    );
}

Expr* Parser::ParseUnary()
{
    if (Check(TokenType::Minus))
    {
        Token op =
            Advance();

        Expr* right =
            ParseUnary();

        return new UnaryExpr(
            op,
            right
        );
    }

    return ParsePrimary();
}

Statement* Parser::ParseStatement()
{
    if (
        Check(TokenType::Float) ||
        Check(TokenType::Int) ||
        Check(TokenType::Bool) ||
        Check(TokenType::Vec2) ||
        Check(TokenType::Vec3) ||
        Check(TokenType::Vec4)
    )
    {
        std::string type =
            Advance().lexeme;

        std::string name =
            Consume(
                TokenType::Identifier,
                "Expected variable name"
            ).lexeme;

        Consume(
            TokenType::Equal,
            "Expected ="
        );

        Expr* init =
            ParseExpression();

        return new VarDeclStmt(
            type,
            name,
            init
        );
    }

    Expr* left =
        ParseExpression();

    Consume(
        TokenType::Equal,
        "Expected ="
    );

    Expr* right =
        ParseExpression();

    return new AssignmentStmt(
        left,
        right
    );
}
