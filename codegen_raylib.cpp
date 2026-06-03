#include "codegen_raylib.h"
#include "codegen.h"

std::string RaylibEmitter::Emit(
    const Program& program)
{
    GLSLGenerator gen;

    std::string vs =
        gen.GenerateVertex(program);

    std::string fs =
        gen.GenerateFragment(program);

    bool hasTime = false;
    bool hasResolution = false;
    bool hasMouse = false;
    bool hasUV = false;

    for (const auto& g : program.globals)
    {
        if (
            g.qualifier == "uniform" &&
            g.type == "float" &&
            g.name == "time"
            )
        {
            hasTime = true;
        }

        if (
            g.qualifier == "uniform" &&
            g.type == "vec2" &&
            g.name == "resolution"
            )
        {
            hasResolution = true;
        }

        if (
            g.qualifier == "uniform" &&
            g.type == "vec2" &&
            g.name == "mouse"
            )
        {
            hasMouse = true;
        }

        if (
            g.qualifier == "varying" &&
            g.type == "vec2" &&
            g.name == "UV"
            )
        {
            hasUV = true;
        }
    }

    std::string cpp;

    cpp += "#include \"raylib.h\"\n\n";

    cpp += "const char* vertexShader = R\"(\n";
    cpp += vs;
    cpp += "\n)\";\n\n";

    cpp += "const char* fragmentShader = R\"(\n";
    cpp += fs;
    cpp += "\n)\";\n\n";

    cpp += R"(

int main()
{
    InitWindow(
        800,
        600,
        "Bark"
    );

    Shader shader =
        LoadShaderFromMemory(
            nullptr,
            fragmentShader
        );
)";

    if (hasTime)
    {
        cpp += R"(

    int timeLoc =
        GetShaderLocation(
            shader,
            "time"
        );
)";
    }
    if (hasResolution)
    {
        cpp += R"(

    int resolutionLoc =
        GetShaderLocation(
            shader,
            "resolution"
        );
)";
    }

    if (hasMouse)
    {
        cpp += R"(

    int mouseLoc =
        GetShaderLocation(
            shader,
            "mouse"
        );
)";
    }

    cpp += R"(

    while (!WindowShouldClose())
    {
)";

    if (hasTime)
    {
        cpp += R"(

        float time =
            (float)GetTime();

        SetShaderValue(
            shader,
            timeLoc,
            &time,
            SHADER_UNIFORM_FLOAT
        );
)";
    }

    if (hasResolution)
    {
        cpp += R"(

        Vector2 resolution =
        {
            (float)GetScreenWidth(),
            (float)GetScreenHeight()
        };

        SetShaderValue(
            shader,
            resolutionLoc,
            &resolution,
            SHADER_UNIFORM_VEC2
        );
)";
    }

    if (hasMouse)
    {
        cpp += R"(

        Vector2 mouse =
        {
            (float)GetMouseX(),
            (float)GetMouseY()
        };

        SetShaderValue(
            shader,
            mouseLoc,
            &mouse,
            SHADER_UNIFORM_VEC2
        );
)";
    }

    cpp += R"(

        BeginDrawing();

        ClearBackground(BLACK);

        BeginShaderMode(shader);

        DrawRectangle(
            0,
            0,
            GetScreenWidth(),
            GetScreenHeight(),
            WHITE
        );

        EndShaderMode();

        EndDrawing();
    }

    UnloadShader(shader);

    CloseWindow();

    return 0;
}
)";

    return cpp;
}