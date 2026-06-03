#pragma once

#include <string>

#include "ast.h"

class GLSLGenerator
{
public:
    std::string GenerateVertex(
        const Program& program);

    std::string GenerateFragment(
        const Program& program);

private:
    std::string GenerateExpr(
        Expr* expr);

    std::string GenerateStatement(
        Statement* stmt);

    std::string GenerateGlobals(
        const Program& program,
        bool vertex);
};