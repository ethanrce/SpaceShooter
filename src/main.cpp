#include <raylib.h>
#include <raymath.h>
#include <fstream>
#include <iostream>

#define FPS 60
#define FRAMESPEED 8

int main(void) {
    int height = 800;
    int width = 800;

    // Limits FPS to refresh rate of monitor
    //SetWindowState(FLAG_VSYNC_HINT);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    // Rectangle = {xf, yf, widthf, heightf}
    // Vector2 (xf, yf)

    InitWindow(height, width, "SpaceShooter");
    SetTargetFPS(FPS);
    
    Texture2D background = LoadTexture("assets/River/background.png");
    NPatchInfo info = {(Rectangle) {0.0f, 0.0f, (float) background.width, (float) background.height}, 0, 0, 0, 0, NPATCH_NINE_PATCH};

    Texture2D ship = LoadTexture("assets/spritesheets/ship.png");
    // Main character sprite animation settings
    Rectangle shipRec = {0.0f, 0.0f, (float)((ship.width/5.0f)), (float)(ship.height/2.0f)};
    int spriteframe = 0;
    int framecounter = 0;

    // Main character position/scaling settings
    float scalerate = (0.1/(shipRec.width/GetScreenWidth())); // Character's width should be 10% of screen.
    Rectangle position = {(float)(GetScreenWidth()/2.0f), (float)(GetScreenHeight()/2.0f), (float)(ship.width/5.0f) * scalerate, (float)(ship.height/2.0f) * scalerate};
    Vector2 origin = {(float)(position.width/2.0f), (float)(position.height/2.0f)};
    float rotation = 0.0f;

    while (!WindowShouldClose()) {
        // Main character sprite animation controller
        framecounter ++;
        shipRec.y = 0.0f;
        if (framecounter >= (FPS/FRAMESPEED)) {
            spriteframe ++;
            framecounter = 0;

            if (spriteframe > 5) {
                spriteframe = 1;
            }

            shipRec.x = ((float)ship.width/5.0f) * spriteframe;
        }

        // TODO: Need to fix borders during window resizing in main menu.
        // TODO: Allow for rotation speed slider in main menu
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
            shipRec.y = ((float)(ship.height/2.0f));
            rotation += 1;
        }
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
            shipRec.y = ((float)(ship.height/2.0f));
            rotation -= 1;
        }
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
         //   if ((float)(position.y) < ((float)((GetScreenHeight()/2.0f)))) {
                origin.y += 3.0f;
                shipRec.y = ((float)(ship.height/2.0f));
         //   }
        }
        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)){
          //  if ((float)(position.y - scale.height) > ((float)(-1*(GetScreenHeight()/2.0f)))) {
                origin.y -= 3.0f;
                shipRec.y = ((float)(ship.height/2.0f));
         //   }
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);
        
        DrawTextureNPatch(background, info, (Rectangle) {0.0f, 0.0f, (float) GetScreenWidth(), (float) GetScreenHeight()}, Vector2Zero(), 0.0f, RAYWHITE); // Draw's background
        DrawTexturePro(ship, shipRec, position, origin, rotation, RAYWHITE); //Draws character
        DrawFPS(10, 10);

        EndDrawing();
    }

    UnloadTexture(background);
    UnloadTexture(ship);
    CloseWindow();

    return 0;
}
