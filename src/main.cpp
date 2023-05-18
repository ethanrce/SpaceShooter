#include <raylib.h>
#include <raymath.h>
#include <fstream>
#include <iostream>


int main(void) {
    int height = 800;
    int width = 800;

    SetWindowState(FLAG_VSYNC_HINT);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(height, width, "SpaceShooter");
    // Limits FPS to refresh rate of monitor


    Texture2D background = LoadTexture("assets/River/background.png");
    NPatchInfo info = { (Rectangle) {0.0f, 0.0f, (float) background.width, (float) background.height}, 0, 0, 0, 0, NPATCH_NINE_PATCH};

    while (!WindowShouldClose()) {

        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawTextureNPatch(background, info, (Rectangle) {0.0f, 0.0f, (float) GetScreenWidth(), (float) GetScreenHeight()}, Vector2Zero(), 0.0f, RAYWHITE);
        DrawFPS(10, 10);

        EndDrawing();
    }

    UnloadTexture(background);
    CloseWindow();

    return 0;
}
