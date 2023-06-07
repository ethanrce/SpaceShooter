#include <raylib.h>
#include <raymath.h>
#include <fstream>
#include <iostream>
using std::cout;
#include "objects.h"
#include <vector>
#include "screens.h"

// static variables
#define FRAMESPEED 8
//TODO: Speed & rotation should change depending on the window size.
#define SHIPSPEED 1.5f
#define SHIPROTATION 3.0f
#define LASERSPEED 5.0f
#define LASERCOOLDOWN 1.0f // Seconds between laser shots
#define ENEMYLASERCOOLDOWN 2.0f // Seconds between enemy laser shots
#define EXPLOSIONCOOLDOWN 8
#define SCREENHEIGHT 800
#define SCREENWIDTH 800
#define FPS 60

// Game texture/Object variables
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

float scale; // Game texture scaling

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


void InitGame(void) {
    LoadTextures();
    spriteframe = 0;
    framecounter = 0;

    lasershootcounter = 0;
    canShoot = true;
    lasercounter = 0;
    laserframe = 0;

    changeFrame = false;
    explosioncounter = 0;
}

void LoadTextures(void) {
    shippng = LoadTexture("assets/spritesheets/ship.png"); 
    // TODO: Need to change scale after fullscreen is enabled/disabled.
    scale = (0.1/((shippng.width/5.0f)/GetScreenWidth())); // Character's width should be 10% of screen. 
    ship = makePlayer(shippng, scale, GetScreenWidth(), GetScreenHeight());

    explosiontext = LoadTexture("assets/spritesheets/explosion.png");
   
    laser = LoadTexture("assets/spritesheets/laser-bolts.png");

    enemys = LoadTexture("assets/spritesheets/enemy-small.png");
    enemys1 = makeEnemy(enemys, scale, 100.0f, GetScreenHeight()/2.0f, "enemy");
    enemys2 = makeEnemy(enemys, scale, GetScreenWidth() - 100, GetScreenHeight()/2.0f, "enemy");
    enemies.push_back(enemys1);
    enemies.push_back(enemys2);

    enemym = LoadTexture("assets/spritesheets/enemy-medium.png");
    enemyl = LoadTexture("assets/spritesheets/enemy-big.png");
}

void UpdateGame(void) {
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
        MakeLaser(ship);
    }

    for (int i = 0; i < (int) enemies.size(); i++) {
        enemies[i].frame ++;
        if (enemies[i].frame >= (float)(FPS/(1.0f/ENEMYLASERCOOLDOWN))) {
            MakeLaser(enemies[i]);
            enemies[i].frame = 0;
        }
    }
}

void DrawGame(void) {
    ClearBackground(RAYWHITE);
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
    // TODO: Decide enemy rotation
    if (enemies.size() != 0) {
        for (int i = 0; i < (int) enemies.size(); i++) {
            DrawTexturePro(enemies[i].texture, enemies[i].drawRec, enemies[i].position, enemies[i].origin, enemies[i].rotation, RAYWHITE);
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
            
            bool eraseLaser = checkCollisions(i);

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
}

void UnloadGame(void) {
    UnloadTexture(shippng);
    UnloadTexture(laser);
    UnloadTexture(enemys);
    UnloadTexture(enemym);
    UnloadTexture(enemyl);
}

int FinishGame(void) {
    return 0;
}

// Constructs a new laser if player can shoot.
// TODO: Make laser shoot from front of ship instead of middle.
void MakeLaser(Object obj) {
    Object newLaser = shootLaser(obj.position.x, obj.position.y, obj.rotation, laser.width, laser.height, scale/2.0f, obj.name);
    lasers.push_back(newLaser);
}

// Check laser's collision with all enemies & player
bool checkCollisions(int index) {
    for (int e = 0; e < (int) enemies.size(); e++) {
        if (CheckCollisionRecs(lasers[index].position, enemies[e].position) && lasers[index].name != enemies[e].name) {
            Object explosion = explodeanim(explosiontext, enemies[e].position, scale);
            explosions.push_back(explosion);
            enemies.erase(enemies.begin() + e);
            if (enemies.size() != 0) {
                e --;    
            }
            return true;
        }             
    }

    if (CheckCollisionRecs(lasers[index].position, ship.position) && lasers[index].name != ship.name) {
        Object explosion = explodeanim(explosiontext, ship.position, scale);
        explosions.push_back(explosion);
        return true;
    }
    return false;
}