#pragma once

#include <string>
#include <vector>
#include "token.h"

struct VariableDecl
{
    std::string qualifier;
    std::string type;
    std::string name;
};

struct Expr
{
    virtual ~Expr() = default;
};

struct LiteralExpr : Expr
{
    std::string value;

    LiteralExpr(const std::string& v)
        : value(v)
    {
    }
};

struct VariableExpr : Expr
{
    std::string name;

    VariableExpr(const std::string& n)
        : name(n)
    {
    }
};

struct UnaryExpr : Expr
{
    Token op;
    Expr* operand;

    UnaryExpr(
        Token o,
        Expr* e)
        :
        op(o),
        operand(e)
    {
    }
};

struct BinaryExpr : Expr
{
    Expr* left;
    Token op;
    Expr* right;

    BinaryExpr(
        Expr* l,
        Token o,
        Expr* r)
        :
        left(l),
        op(o),
        right(r)
    {
    }
};

struct CallExpr : Expr
{
    std::string name;

    std::vector<Expr*> args;

    CallExpr(
        const std::string& n,
        const std::vector<Expr*>& a)
        :
        name(n),
        args(a)
    {
    }
};

struct MemberExpr : Expr
{
    Expr* object;

    std::string member;

    MemberExpr(
        Expr* obj,
        const std::string& mem)
        :
        object(obj),
        member(mem)
    {
    }
};

struct Statement
{
    virtual ~Statement() = default;
};

struct VarDeclStmt : Statement
{
    std::string type;
    std::string name;

    Expr* initializer;

    VarDeclStmt(
        const std::string& t,
        const std::string& n,
        Expr* init)
        :
        type(t),
        name(n),
        initializer(init)
    {
    }
};

struct AssignmentStmt : Statement
{
    Expr* target;
    Expr* value;

    AssignmentStmt(
        Expr* t,
        Expr* v)
        :
        target(t),
        value(v)
    {
    }
};

struct ShaderBlock
{
    std::vector<Statement*> statements;
};

struct Program
{
    std::vector<VariableDecl> globals;

    ShaderBlock vertex;
    ShaderBlock fragment;
};