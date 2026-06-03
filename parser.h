#pragma once

#include <vector>

#include "token.h"
#include "ast.h"

class Parser
{
private:
    std::vector<Token> tokens;
    size_t current = 0;

    Token& Peek();
    Token& Previous();
    bool IsAtEnd();
    Token Advance();
    bool Check(TokenType type);
    bool Match(TokenType type);
    Token Consume(TokenType expected, const std::string& message);

    Expr* ParseExpression();

    Expr* ParsePrimary();

    Statement* ParseStatement();

    Expr* ParseAddition();

    Expr* ParseMultiplication();

    Expr* ParseUnary();

public:
    Parser(const std::vector<Token>& toks);

    VariableDecl ParseDeclaration();
    ShaderBlock ParseShaderBlock();
    Program Parse();
};

