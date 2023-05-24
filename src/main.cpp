#include <raylib.h>
#include <raymath.h>
#include <fstream>
#include <iostream>
using namespace std;
#include "player.h"
#include <vector>

#define FPS 60
#define FRAMESPEED 8
#define SHIPSPEED 1.5f
#define LASERSPEED 2.0f
#define LASERCOOLDOWN 1
// Rectangle = {xf, yf, widthf, heightf}
// Vector2 (xf, yf)

int main(void) {
    int height = 400;
    int width = 400;

    // Limits FPS to refresh rate of monitor
    //SetWindowState(FLAG_VSYNC_HINT);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

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

    // Laser position/animation settings
    Texture2D laser = LoadTexture("assets/spritesheets/laser-bolts.png");
    vector<Laser> lasers;
    int lasercounter = 0;
    int laserframe = 0;
    int laserx;
    int lasery;

    // Laser cooldown settings
    int lasershootcounter = 0;
    bool canShoot = true;

    Texture2D enemys = LoadTexture("assets/spritesheets/enemy-small.png");
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
            if (position.x < (GetScreenWidth() - (position.width/2.0f))) {
            shipRec.y = (ship.height/2.0f);
            position.x += SHIPSPEED;
            rotation = 90.0f;
            }
        }
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
            if (position.x > (position.width/2.0f)) {
            shipRec.y = (ship.height/2.0f);
            position.x -= SHIPSPEED;
            rotation = 270.0f;
            }
        }
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
            if (position.y > (position.height/2.0f)) {
                position.y -= SHIPSPEED;
                rotation = 0.0f;
                shipRec.y = ship.height/2.0f;
            }
        }
        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)){
            if (position.y < (GetScreenHeight() - (position.height/2.0f))) {
                position.y += SHIPSPEED;
                rotation = 180.0f;
                shipRec.y = (ship.height/2.0f);
            }
        }

        // Controls laser shooting cooldown
        lasershootcounter ++;
        if (lasershootcounter >= (FPS/LASERCOOLDOWN) && !canShoot) {
            lasershootcounter = 0;
            canShoot = true;
        }

        // Controls initiating shooting & initial position of laser
        if (IsKeyPressed(KEY_F) && canShoot) {
            canShoot = false;
            if (rotation == 0.0f) {
                laserx = position.x;
                lasery = position.y - (position.height/2.0f);
            } else if (rotation == 90.0f) {
                laserx = position.x + (position.width/2.0f);
                lasery = position.y;
            } else if (rotation == 180.0f) {
                laserx = position.x;
                lasery = position.y + (position.height/2.0f);
            } else {
                laserx = position.x - (position.width/2.0f);
                lasery = position.y;
            }
           Laser newLaser = shootLaser(laserx, lasery, rotation, laser.width, laser.height, scalerate/2.0f);
           lasers.push_back(newLaser);
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);
        
        DrawTextureNPatch(background, info, (Rectangle) {0.0f, 0.0f, (float) GetScreenWidth(), (float) GetScreenHeight()}, Vector2Zero(), 0.0f, RAYWHITE); // Draw's background
        DrawTexturePro(ship, shipRec, position, origin, rotation, RAYWHITE); //Draws character

        // Controls laser drawing
        // TODO: Need to change laser's orientation depending on ship rotation.
        // TODO: Need to remove laser once out of window
        if (lasers.size() != 0) {
            lasercounter ++;
            if (lasercounter >= (FPS/FRAMESPEED)) {
                lasercounter = 0;
                laserframe ++;
                if (laserframe >= 2) {
                    laserframe = 0;
                }
            }
            for (int i = 0; i < lasers.size(); i++) {
                lasers[i].laserRec.x = ((float)(laser.width/2.0f) * laserframe);
                if (lasers[i].rotation == 0.0f) {
                    lasers[i].position.y -= LASERSPEED;
                } else if (lasers[i].rotation == 90.0f) {
                    lasers[i].position.x += LASERSPEED;
                } else if (lasers[i].rotation == 180.0f) {
                    lasers[i].position.y += LASERSPEED;
                } else {
                    lasers[i].position.x -= LASERSPEED;
                }
                DrawTexturePro(laser, lasers[i].laserRec, lasers[i].position, lasers[i].origin, lasers[i].rotation, RAYWHITE);
                // Dequeue's laser after it's off screen
                if (lasers[i].position.x > GetScreenWidth() || lasers[i].position.x < 0 || lasers[i].position.y > GetScreenHeight() || lasers[i].position.y < 0) {
                    lasers.erase(lasers.begin() + i - 1);
                }
            }
        }
        cout << lasers.size() << endl;

        DrawFPS(10, 10);

        EndDrawing();
    }

    UnloadTexture(background);
    UnloadTexture(ship);
    UnloadTexture(laser);
    UnloadTexture(enemys);
    CloseWindow();

    return 0;
}
