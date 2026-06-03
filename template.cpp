#include "raylib.h"

const char* vertexShader = R"(

... generated vertex shader ...

)";

const char* fragmentShader = R"(

... generated fragment shader ...

)";

int main()
{
    InitWindow(
        800,
        600,
        "Bark"
    );

    Shader shader =
        LoadShaderFromMemory(
            vertexShader,
            fragmentShader
        );

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);

        BeginShaderMode(shader);

        DrawRectangle(
            0,
            0,
            800,
            600,
            WHITE
        );

        EndShaderMode();

        EndDrawing();
    }

    UnloadShader(shader);

    CloseWindow();
}