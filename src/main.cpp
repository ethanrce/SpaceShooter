#include <raylib.h>
#include <raymath.h>
#include <fstream>
#include <iostream>


int main(void) {
    int height = 800;
    int width = 800;

    InitWindow(height, width, "SpaceShooter");
    // Limits FPS to refresh rate of monitor
    SetWindowState(FLAG_VSYNC_HINT);

    Texture2D background = LoadTexture("assets/River/background.png");
    //std::cout << "Hello world";

    while (!WindowShouldClose()) {

        BeginDrawing();

        DrawTextureEx(background, GetWindowPosition(), 0.0f, 2.0f, WHITE);
        DrawFPS(10, 10);

        EndDrawing();
    }


    CloseWindow();

    return 0;
}
