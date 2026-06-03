#include "raylib.h"

const char* vertexShader = R"(
#version 330

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

const char* fragmentShader = R"(
#version 330

uniform vec2 resolution;
uniform vec2 mouse;
out vec4 color;

void main()
{
    color = vec4((mouse.x / resolution.x), (mouse.y / resolution.y), 0.0, 1.0);
}

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
            nullptr,
            fragmentShader
        );


    int resolutionLoc =
        GetShaderLocation(
            shader,
            "resolution"
        );


    int mouseLoc =
        GetShaderLocation(
            shader,
            "mouse"
        );


    while (!WindowShouldClose())
    {


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
