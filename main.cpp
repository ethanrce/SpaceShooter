#include <raylib.h>
#include <raymath.h>
#include <fstream>
#include <iostream>
using namespace std;
#include "objects.h"
#include <vector>

#define FPS 60
#define FRAMESPEED 8
#define SHIPSPEED 1.5f
#define LASERSPEED 2.0f
#define LASERCOOLDOWN 1.0f // Seconds between laser shots
#define SCREENHEIGHT 800
#define SCREENWIDTH 800

// Game texture/Object variables
Texture2D background;
NPatchInfo info;
Texture2D shippng;
Object ship;
Texture2D laser;
vector<Object> lasers;
Texture2D enemys;
Texture2D enemym;
Texture2D enemyl;

// Game texture scaling
float scale;

// Main character sprite animation settings
int spriteframe;
int framecounter;

// Laser cooldown, animation & position settings
int lasershootcounter;
bool canShoot;
int lasercounter;
int laserframe;
int laserx;
int lasery; 

// Rectangle = {xf, yf, widthf, heightf}
// Vector2 (xf, yf)

// Loads all necessary textures for the game
void LoadTextures(void) {
    background = LoadTexture("assets/River/background.png");
    info = {(Rectangle) {0.0f, 0.0f, (float) background.width, (float) background.height}, 0, 0, 0, 0, NPATCH_NINE_PATCH};

    shippng = LoadTexture("assets/spritesheets/ship.png");
    scale = (0.1/((shippng.width/5.0f)/GetScreenWidth())); // Character's width should be 10% of screen.
    ship = makePlayer(shippng, scale, GetScreenWidth(), GetScreenHeight());

    laser = LoadTexture("assets/spritesheets/laser-bolts.png");

    enemys = LoadTexture("assets/spritesheets/enemy-small.png");
    enemym = LoadTexture("assets/spritesheets/enemy-medium.png");
    enemyl = LoadTexture("assets/spritesheets/enemy-big.png");
}

// Initializes game variables/settings
void InitGame(void) {
    SetTargetFPS(FPS);

    spriteframe = 0;
    framecounter = 0;

    lasershootcounter = 0;
    canShoot = true;
    lasercounter = 0;
    laserframe = 0;
}

// Constructs a new laser if player can shoot
void makeLaser(void) {
    if (ship.rotation == 0.0f) {
        laserx = ship.position.x;
        lasery = ship.position.y - (ship.position.height/2.0f);
    } else if (ship.rotation == 90.0f) {
        laserx = ship.position.x + (ship.position.width/2.0f);
        lasery = ship.position.y;
    } else if (ship.rotation == 180.0f) {
        laserx = ship.position.x;
        lasery = ship.position.y + (ship.position.height/2.0f);
    } else {
        laserx = ship.position.x - (ship.position.width/2.0f);
        lasery = ship.position.y;
    }
    Object newLaser = shootLaser(laserx, lasery, ship.rotation, laser.width, laser.height, scale/2.0f);
    lasers.push_back(newLaser);
}

// Unloads all load textures, sounds, models, etc.
void UnloadGame(void) {
    UnloadTexture(background);
    UnloadTexture(shippng);
    UnloadTexture(laser);
    UnloadTexture(enemys);
    UnloadTexture(enemym);
    UnloadTexture(enemyl);
}

int main(void) {
    // Limits FPS to refresh rate of monitor
    //SetWindowState(FLAG_VSYNC_HINT);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(SCREENHEIGHT, SCREENWIDTH, "SpaceShooter");

    LoadTextures();
    InitGame();

    while (!WindowShouldClose()) {
        // Main character sprite animation controller
        framecounter ++;
        ship.drawRec.y = 0.0f;
        if (framecounter >= (FPS/FRAMESPEED)) {
            spriteframe ++;
            framecounter = 0;

            if (spriteframe > 5) {
                spriteframe = 1;
            }

            ship.drawRec.x = ((float)shippng.width/5.0f) * spriteframe;
        }

        // TODO: Need to fix borders during window resizing in main menu.
        // TODO: Allow for rotation speed slider in main menu
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
            if (ship.position.x < (GetScreenWidth() - (ship.position.width/2.0f))) {
            ship.drawRec.y = (shippng.height/2.0f);
            ship.position.x += SHIPSPEED;
            ship.rotation = 90.0f;
            }
        }
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
            if (ship.position.x > (ship.position.width/2.0f)) {
            ship.drawRec.y = (shippng.height/2.0f);
            ship.position.x -= SHIPSPEED;
            ship.rotation = 270.0f;
            }
        }
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
            if (ship.position.y > (ship.position.height/2.0f)) {
                ship.position.y -= SHIPSPEED;
                ship.rotation = 0.0f;
                ship.drawRec.y = shippng.height/2.0f;
            }
        }
        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)){
            if (ship.position.y < (GetScreenHeight() - (ship.position.height/2.0f))) {
                ship.position.y += SHIPSPEED;
                ship.rotation = 180.0f;
                ship.drawRec.y = shippng.height/2.0f;
            }
        }

        // Controls laser shooting cooldown
        lasershootcounter ++;
        if (lasershootcounter >= ((float)(FPS/(1.0f/LASERCOOLDOWN))) && !canShoot) {
            canShoot = true;
        }

        // Controls initiating shooting & initial position of laser
        if (IsKeyPressed(KEY_F) && canShoot) {
            canShoot = false;
            lasershootcounter = 0;
            makeLaser();
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);
        
        DrawTextureNPatch(background, info, (Rectangle) {0.0f, 0.0f, (float) GetScreenWidth(), (float) GetScreenHeight()}, Vector2Zero(), 0.0f, RAYWHITE); // Draw's background
        DrawTexturePro(shippng, ship.drawRec, ship.position, ship.origin, ship.rotation, RAYWHITE); //Draws character
     //   DrawTexturePro(enemys, esRec, esposition, esorigin, 0.0f, RAYWHITE);

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
            for (int i = 0; i < (int)lasers.size(); i++) {
                lasers[i].drawRec.x = ((float)(laser.width/2.0f) * laserframe);
                if (lasers[i].rotation == 0.0f) {
                    lasers[i].position.y -= LASERSPEED;
                } else if (lasers[i].rotation == 90.0f) {
                    lasers[i].position.x += LASERSPEED;
                } else if (lasers[i].rotation == 180.0f) {
                    lasers[i].position.y += LASERSPEED;
                } else {
                    lasers[i].position.x -= LASERSPEED;
                }
                DrawTexturePro(laser, lasers[i].drawRec, lasers[i].position, lasers[i].origin, lasers[i].rotation, RAYWHITE);
                // Dequeue's laser after it's off screen
                if (lasers[i].position.x > GetScreenWidth() || lasers[i].position.x < 0 || lasers[i].position.y > GetScreenHeight() || lasers[i].position.y < 0) {
                    lasers.erase(lasers.begin() + i - 1);
                }
            }
        }

        DrawFPS(10, 10);

        EndDrawing();
    }

    UnloadGame();

    CloseWindow();

    return 0;
}