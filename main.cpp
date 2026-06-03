#include <iostream>
#include <fstream>
#include <sstream>
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "codegen.h"
#include "codegen_raylib.h"

void PrintExpr(Expr* expr, int indent = 0)
{
    std::string pad(indent, ' ');

    if (auto lit = dynamic_cast<LiteralExpr*>(expr))
    {
        std::cout << pad
                  << "Literal: "
                  << lit->value
                  << "\n";
    }
    else if (auto var = dynamic_cast<VariableExpr*>(expr))
    {
        std::cout << pad
                  << "Variable: "
                  << var->name
                  << "\n";
    }
    else if (auto unary = dynamic_cast<UnaryExpr*>(expr))
    {
        std::cout << pad
                  << "Unary: "
                  << unary->op.lexeme
                  << "\n";

        PrintExpr(unary->operand, indent + 4);
    }
    else if (auto binary = dynamic_cast<BinaryExpr*>(expr))
    {
        std::cout << pad
                  << "Binary: "
                  << binary->op.lexeme
                  << "\n";

        PrintExpr(binary->left, indent + 4);
        PrintExpr(binary->right, indent + 4);
    }
    else if (auto call = dynamic_cast<CallExpr*>(expr))
    {
        std::cout << pad
                  << "Call: "
                  << call->name
                  << "\n";

        for (auto arg : call->args)
        {
            PrintExpr(arg, indent + 4);
        }
    }
    else if (auto member = dynamic_cast<MemberExpr*>(expr))
    {
        std::cout << pad
                  << "Member: "
                  << member->member
                  << "\n";

        PrintExpr(member->object, indent + 4);
    }
}

void PrintStatement(Statement* stmt, int indent = 0)
{
    std::string pad(indent, ' ');

    if (auto decl = dynamic_cast<VarDeclStmt*>(stmt))
    {
        std::cout << pad
                  << "VarDecl\n";

        std::cout << pad
                  << "  Type: "
                  << decl->type
                  << "\n";

        std::cout << pad
                  << "  Name: "
                  << decl->name
                  << "\n";

        PrintExpr(
            decl->initializer,
            indent + 4
        );
    }
    else if (auto assign =
             dynamic_cast<AssignmentStmt*>(stmt))
    {
        std::cout << pad
                  << "Assignment\n";

        std::cout << pad
                  << "  Target:\n";

        PrintExpr(
            assign->target,
            indent + 4
        );

        std::cout << pad
                  << "  Value:\n";

        PrintExpr(
            assign->value,
            indent + 4
        );
    }
}

void PrintBlock(
    const ShaderBlock& block,
    const std::string& name)
{
    std::cout
        << "\n=== "
        << name
        << " ===\n";

    for (auto stmt : block.statements)
    {
        PrintStatement(stmt, 4);
    }
}

void PrintProgram(const Program& p)
{
    std::cout
        << "=== GLOBALS ===\n";

    for (const auto& g : p.globals)
    {
        std::cout
            << g.qualifier << " "
            << g.type << " "
            << g.name << "\n";
    }

    PrintBlock(
        p.vertex,
        "VERTEX"
    );

    PrintBlock(
        p.fragment,
        "FRAGMENT"
    );
}

int main()
{
    std::ifstream file("triangle.bark");
    if (!file)
    {
        throw std::runtime_error(
            "Could not open file"
        );
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string source =
        buffer.str();
    try
    {
        Tokenizer lexer(source);

        auto tokens =
            lexer.Tokenize();

        for (const auto& token : tokens)
        {
            std::cout
                << TokenName(token.type)
                << " : "
                << token.lexeme
                << "\n";
        }

        Parser parser(tokens);

        Program program =
            parser.Parse();

        RaylibEmitter emitter;

        std::string generatedSource =
            emitter.Emit(program);

        std::ofstream out(
            "generated.cpp"
        );

        out << generatedSource;
    }
    catch (const std::exception& e)
    {
        std::cout
            << "ERROR: "
            << e.what()
            << "\n";
    }

    return 0;
}