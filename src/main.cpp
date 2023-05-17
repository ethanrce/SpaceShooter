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

    //std::cout << "Hello world";

    while (!WindowShouldClose()) {

        BeginDrawing();
        
        ClearBackground(RAYWHITE);
        const char *background = "assets/River/background.png";
        Texture2D background2 = LoadTexture(background);
        DrawTexture(background2, height/2, width/2, RAYWHITE);

        LoadImageFromScreen();
        DrawFPS(10, 10);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
