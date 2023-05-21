#include <raylib.h>
#include <raymath.h>
#include <fstream>
#include <iostream>


int main(void) {
    int height = 800;
    int width = 800;

    // Limits FPS to refresh rate of monitor
    //SetWindowState(FLAG_VSYNC_HINT);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    // Rectangle = {xf, yf, widthf, heightf}
    // Vector2 (xf, yf)

    InitWindow(height, width, "SpaceShooter");
    int FPS = 60;
    SetTargetFPS(FPS);

    Texture2D background = LoadTexture("assets/River/background.png");
    NPatchInfo info = {(Rectangle) {0.0f, 0.0f, (float) background.width, (float) background.height}, 0, 0, 0, 0, NPATCH_NINE_PATCH};

    
    Texture2D ship = LoadTexture("assets/spritesheets/ship.png");
    // Main character sprite animation settings
    Rectangle shipRec = {0, 0, (float)((ship.width/5.0f)), (float)(ship.height/2.0f)};
    int spriteframe = 0;
    int framespeed = 8;
    int framecounter = 0;

    while (!WindowShouldClose()) {
        // Main character sprite animation controller
        framecounter ++;
        if (framecounter >= (FPS/framespeed)) {
            spriteframe ++;
            framecounter = 0;

            if (spriteframe > 5) {
                spriteframe = 1;
            }

            shipRec.x = ((float)ship.width/5) * spriteframe;
        }

        // Main character sprite scaling settings
        float scalerate = 7.0f;
        Rectangle scale = {(float)(GetScreenWidth()/2.0), (float)(GetScreenHeight()/2.0), (float)(ship.width/5.0f) * scalerate, (float)(ship.height/2.0f) * scalerate};
        Vector2 origin = {(float)(scale.width/2.0f), (float)(scale.height/2.0f)};

        BeginDrawing();

        ClearBackground(RAYWHITE);
        
        DrawTextureNPatch(background, info, (Rectangle) {0.0f, 0.0f, (float) GetScreenWidth(), (float) GetScreenHeight()}, Vector2Zero(), 0.0f, RAYWHITE); // Draw's background
        DrawTexturePro(ship, shipRec, scale, origin, 0.0f, RAYWHITE); //Draws character
        DrawFPS(10, 10);

        EndDrawing();
    }

    UnloadTexture(background);
    UnloadTexture(ship);
    CloseWindow();

    return 0;
}
