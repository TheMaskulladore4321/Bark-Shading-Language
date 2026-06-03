#include "codegen.h"
#include <stdexcept>

std::string GLSLGenerator::GenerateExpr(Expr* expr)
{
    if (auto lit =
        dynamic_cast<LiteralExpr*>(expr))
    {
        return lit->value;
    }

    if (auto var =
        dynamic_cast<VariableExpr*>(expr))
    {
        return var->name;
    }

    if (auto unary =
        dynamic_cast<UnaryExpr*>(expr))
    {
        return unary->op.lexeme +
            GenerateExpr(
                unary->operand
            );
    }

    if (auto bin =
        dynamic_cast<BinaryExpr*>(expr))
    {
        return "("
            + GenerateExpr(bin->left)
            + " "
            + bin->op.lexeme
            + " "
            + GenerateExpr(bin->right)
            + ")";
    }

    if (auto call =
        dynamic_cast<CallExpr*>(expr))
    {
        std::string result =
            call->name + "(";

        for (size_t i = 0;
            i < call->args.size();
            i++)
        {
            result +=
                GenerateExpr(
                    call->args[i]
                );

            if (i + 1 < call->args.size())
                result += ", ";
        }

        result += ")";

        return result;
    }

    if (auto member =
        dynamic_cast<MemberExpr*>(expr))
    {
        return
            GenerateExpr(
                member->object
            )
            + "."
            + member->member;
    }
    throw std::runtime_error(
        "Unknown expression type"
    );
}

std::string GLSLGenerator::GenerateStatement(Statement* stmt)
{
    if (auto decl =
        dynamic_cast<VarDeclStmt*>(stmt))
    {
        return
            decl->type
            + " "
            + decl->name
            + " = "
            + GenerateExpr(
                decl->initializer
            )
            + ";";
    }

    if (auto assign =
        dynamic_cast<AssignmentStmt*>(stmt))
    {
        return
            GenerateExpr(
                assign->target
            )
            + " = "
            + GenerateExpr(
                assign->value
            )
            + ";";
    }
    throw std::runtime_error(
        "Unknown statement type"
    );
}

std::string GLSLGenerator::GenerateGlobals(const Program& program, bool vertex)
{
    std::string result;

    for (const auto& g : program.globals)
    {
        if (g.qualifier == "uniform")
        {
            result +=
                "uniform "
                + g.type
                + " "
                + g.name
                + ";\n";
        }

        else if (g.qualifier == "varying")
        {
            result +=
                (vertex ? "out " : "in ");

            result +=
                g.type
                + " "
                + g.name
                + ";\n";
        }

        else if (g.qualifier == "input")
        {
            if (vertex)
            {
                result +=
                    "in "
                    + g.type
                    + " "
                    + g.name
                    + ";\n";
            }
        }

        else if (g.qualifier == "output")
        {
            if (!vertex)
            {
                result +=
                    "out "
                    + g.type
                    + " "
                    + g.name
                    + ";\n";
            }
        }
    }

    return result;
}

std::string GLSLGenerator::GenerateFragment(
    const Program& program)
{
    std::string glsl;

    glsl += "#version 330\n\n";

    glsl +=
        GenerateGlobals(
            program,
            false
        );

    glsl += "\n";

    glsl +=
        "void main()\n{\n";

    for (auto stmt :
         program.fragment.statements)
    {
        glsl += "    ";

        glsl +=
            GenerateStatement(
                stmt
            );

        glsl += "\n";
    }

    glsl += "}\n";

    return glsl;
}

std::string GLSLGenerator::GenerateVertex(
    const Program& program)
{
    std::string glsl;

    glsl += "#version 330\n\n";

    glsl +=
        GenerateGlobals(
            program,
            true
        );

    glsl += "\n";

    glsl +=
        "void main()\n{\n";

    for (auto stmt :
         program.vertex.statements)
    {
        glsl += "    ";

        glsl +=
            GenerateStatement(
                stmt
            );

        glsl += "\n";
    }

    glsl += "}\n";

    if (program.vertex.statements.empty())
    {
        return R"(#version 330

            in vec3 vertexPosition;
            in vec2 vertexTexCoord;

            out vec2 UV;

            uniform mat4 mvp;

            void main()
            {
                UV = vertexTexCoord;

                gl_Position =
                    mvp *
                    vec4(
                        vertexPosition,
                        1.0
                    );
            }
        }
        )";
    }

    return glsl;
}