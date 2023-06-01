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
#define SHIPROTATION 5.0f
#define LASERSPEED 2.0f
#define LASERCOOLDOWN 1.0f // Seconds between laser shots
#define EXPLOSIONCOOLDOWN 8
#define SCREENHEIGHT 800
#define SCREENWIDTH 800

// Game texture/Object variables
Texture2D background;
NPatchInfo info;
Texture2D shippng;
Object ship;
Texture2D laser;
vector<Object> lasers;
vector<Object> enemies;
Texture explosiontext;
vector<Object> explosions;
Texture2D enemys;
Object enemys1; // Temp enemy for testing
Object enemys2;
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

// Explosion animation settings
bool changeFrame;
int explosioncounter;

// Rectangle = {xf, yf, widthf, heightf}
// Vector2 (xf, yf)

// Loads all necessary textures for the game
void LoadTextures(void) {
    background = LoadTexture("assets/River/background.png");
    info = {(Rectangle) {0.0f, 0.0f, (float) background.width, (float) background.height}, 0, 0, 0, 0, NPATCH_NINE_PATCH};

    shippng = LoadTexture("assets/spritesheets/ship.png");
    scale = (0.1/((shippng.width/5.0f)/GetScreenWidth())); // Character's width should be 10% of screen.
    ship = makePlayer(shippng, scale, GetScreenWidth(), GetScreenHeight());

    explosiontext = LoadTexture("assets/spritesheets/explosion.png");
   
    laser = LoadTexture("assets/spritesheets/laser-bolts.png");

    enemys = LoadTexture("assets/spritesheets/enemy-small.png");
    enemys1 = makeSmallEnemy(enemys, scale);
    enemys2 = makeSmallEnemy(enemys, scale);
    enemys2.position.x = (float(GetScreenWidth() - 20));
    enemies.push_back(enemys1);
    enemies.push_back(enemys2);

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

    changeFrame = false;
    explosioncounter = 0;
}

// Constructs a new laser if player can shoot.
// TODO: Make laser shoot from front of ship instead of middle.
void makeLaser(void) {
    Object newLaser = shootLaser(ship.position.x, ship.position.y, ship.rotation, laser.width, laser.height, scale/2.0f);
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
        explosioncounter ++;
        ship.drawRec.y = 0.0f;
        if (framecounter >= (FPS/FRAMESPEED)) {
            spriteframe ++;
            framecounter = 0;

            if (spriteframe > 5) {
                spriteframe = 1;
            }
            ship.drawRec.x = ((float)shippng.width/5.0f) * spriteframe;
        }

        // TODO: Need to fix borders during window resizing in main menu. (depends on itch.io structure)
        // TODO: Allow for rotation speed slider in main menu
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
            if (ship.position.x < (GetScreenWidth() - (ship.position.width/2.0f))) {
                ship.drawRec.y = (shippng.height/2.0f);
                ship.rotation += SHIPROTATION;
            }
        }
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
            if (ship.position.x > (ship.position.width/2.0f)) {
                ship.drawRec.y = (shippng.height/2.0f);
                ship.rotation -= SHIPROTATION;
            }
        }
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
            if (ship.position.y > (ship.position.height/2.0f)) {
                ship.drawRec.y = shippng.height/2.0f;
                ship.position.x += SHIPSPEED * sinf(ship.rotation * (PI / 180.0f));
                ship.position.y -= SHIPSPEED * cosf(ship.rotation * (PI / 180.0f));
            }
        }
        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)){
            if (ship.position.y < (GetScreenHeight() - (ship.position.height/2.0f))) {
                ship.drawRec.y = shippng.height/2.0f;
                ship.position.x -= SHIPSPEED * sinf(ship.rotation * (PI / 180.0f));
                ship.position.y += SHIPSPEED * cosf(ship.rotation * (PI / 180.0f));
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
            explosioncounter = 0;
            makeLaser();
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);
        
        DrawTextureNPatch(background, info, (Rectangle) {0.0f, 0.0f, (float) GetScreenWidth(), (float) GetScreenHeight()}, Vector2Zero(), 0.0f, RAYWHITE); // Draw's background
        DrawTexturePro(shippng, ship.drawRec, ship.position, ship.origin, ship.rotation, RAYWHITE); //Draws character

        // Controls explosion drawing
        if (explosions.size() != 0) {
            if (explosioncounter >= (FPS/EXPLOSIONCOOLDOWN)) {
                changeFrame = true;
            }
            for (int i = 0; i < (int) explosions.size(); i++) {
                if (changeFrame) {
                    changeFrame = false;
                    explosioncounter = 0;
                    explosions[i].frame ++; 
                }
                explosions[i].drawRec.x = ((float)((explosiontext.width/5.0f) * explosions[i].frame));
                if (explosions[i].frame > 5) {
                    explosions.erase(explosions.begin() + i);
                    if (explosions.size() != 0) {
                        i --;
                    }  
                } else {
                    DrawTexturePro(explosiontext, explosions[i].drawRec, explosions[i].position, explosions[i].origin, explosions[i].rotation, RAYWHITE);
                }
            }
        } else {
            explosioncounter = 0;
        }

        // Controls enemy drawing
        if (enemies.size() != 0) {
            for (int i = 0; i < (int) enemies.size(); i++) {
                float relativex = enemies[i].position.x - ship.position.x;
                if (relativex >= 0.0f) {
                    enemies[i].rotation = 90.0f;
                } else {
                    enemies[i].rotation = 270.0f;
                }
                DrawTexturePro(enemys, enemies[i].drawRec, enemies[i].position, enemies[i].origin, enemies[i].rotation, RAYWHITE);
            }
        }

        // Controls laser drawing
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
                lasers[i].position.x += SHIPSPEED * sinf(lasers[i].rotation * (PI / 180.0f));
                lasers[i].position.y -= SHIPSPEED * cosf(lasers[i].rotation * (PI / 180.0f));
                DrawTexturePro(laser, lasers[i].drawRec, lasers[i].position, lasers[i].origin, lasers[i].rotation, RAYWHITE);
                
                bool eraseLaser = false;

                // Check laser's collision with all enemies
                if (enemies.size() != 0) { 
                    for (int e = 0; e < (int) enemies.size(); e++) {
                            if (CheckCollisionRecs(lasers[i].position, enemies[e].position)) {
                                Object explosion = explodeanim(explosiontext, enemies[e].position, scale);
                                explosions.push_back(explosion);
                                enemies.erase(enemies.begin() + e);
                                eraseLaser = true;   
                                if (enemies.size() != 0) {
                                    e --;    
                                }
                            }             
                    }
                }
                
                // Dequeues lasers
                if (eraseLaser || (lasers[i].position.x > GetScreenWidth() || lasers[i].position.x < 0 || lasers[i].position.y > GetScreenHeight() || lasers[i].position.y < 0)) {
                    lasers.erase(lasers.begin() + i); 
                    if (lasers.size() != 0) {
                        i --;
                    }
                }
                
                
                
            }
        } else {
            lasercounter = 0;
        }

        DrawFPS(10, 10);

        EndDrawing();
    }

    UnloadGame();

    CloseWindow();

    return 0;
}