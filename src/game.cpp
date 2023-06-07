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
#define SHIPSPEED 2.5f
#define SHIPROTATION 3.0f
#define LASERSPEED 5.0f
#define LASERCOOLDOWN 1.0f // Seconds between laser shots
#define ENEMYSSPEED 1.5f
#define ENEMYLASERCOOLDOWN 2.0f // Seconds between enemy laser shots
#define SCREENHEIGHT 800
#define SCREENWIDTH 800

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
int laserframe;

int enemyframe;

int display;
int fps;

void InitGame(void) {
    LoadTextures();
    spriteframe = 0;
    framecounter = 0;

    lasershootcounter = 0;
    canShoot = true;
    laserframe = 0;
    
    enemyframe = 0;

    display = GetCurrentMonitor();
    fps = GetMonitorRefreshRate(display);
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
    // Controls object animations
    framecounter ++;
    ship.drawRec.y = 0.0f;
    if (framecounter >= (fps/FRAMESPEED)) {
        spriteframe ++;
        framecounter = 0;

        // Player
        if (spriteframe >= 5) {
            spriteframe = 0;
        }
        ship.drawRec.x = ((float)shippng.width/5.0f) * spriteframe;

        // Enemies
        if (enemies.size() > 0) {
            enemyframe ++;
            if (enemyframe >= 2) {
                enemyframe = 0;
            }
            for (int i = 0; i < (int) enemies.size(); i++) {
                enemies[i].drawRec.x = ((float)(enemies[i].texture.width/2.0f) * enemyframe);
            }
        } else {
            enemyframe = 0;
        }

        // Lasers
        if (lasers.size() > 0) {
            laserframe ++;
            if (laserframe >= 2) {
                laserframe = 0;
            }
            for (int i = 0; i < (int) lasers.size(); i++) {
                lasers[i].drawRec.x = ((float)(laser.width/2.0f) * laserframe);
            }
        } else {
            laserframe = 0;
        }
        
        // Explosions
        if (explosions.size() > 0) {
            for (int i = 0; i < (int) explosions.size(); i++) {
                explosions[i].frame ++;
                explosions[i].drawRec.x = ((float)((explosiontext.width/5.0f) * explosions[i].frame));
                if (explosions[i].frame >= 5) {
                    explosions.erase(explosions.begin() + i);
                    if (explosions.size() != 0) {
                        i --;
                    } 
                }
            }
        }
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

    // Controls laser position updating each frame & collision checking
    for (int i = 0; i < (int) lasers.size(); i++) {
        lasers[i].position.x += LASERSPEED * sinf(lasers[i].rotation * (PI / 180.0f));
        lasers[i].position.y -= LASERSPEED * cosf(lasers[i].rotation * (PI / 180.0f));

        bool eraseLaser = checkCollisions(i);

        // Dequeues lasers
        if (eraseLaser || (lasers[i].position.x > GetScreenWidth() || lasers[i].position.x < 0 || lasers[i].position.y > GetScreenHeight() || lasers[i].position.y < 0)) {
            lasers.erase(lasers.begin() + i); 
            if (lasers.size() != 0) {
                i --;
            }
        }
    }
 
    // Controls player laser shooting cooldown
    lasershootcounter ++;
    if (lasershootcounter >= ((float)(fps/(1.0f/LASERCOOLDOWN))) && !canShoot) {
        canShoot = true;
    }

    // Controls initiating shooting & initial position of laser
    if (IsKeyPressed(KEY_F) && canShoot) {
        canShoot = false;
        lasershootcounter = 0;
        MakeLaser(ship);
    }
     
    // Controls enemy laser shooting cooldown and updating position
    for (int i = 0; i < (int) enemies.size(); i++) {
        enemies[i].frame ++;
        if (enemies[i].frame >= (float)(fps/(1.0f/ENEMYLASERCOOLDOWN))) {
            MakeLaser(enemies[i]);
            enemies[i].frame = 0;
        }
        float axis = RandomNum(0,1);
        cout << axis << endl;
        MoveRandomly(i, axis);
        cout << "Finish" << endl;
    }
}

void DrawGame(void) {
    ClearBackground(RAYWHITE);
    DrawTexturePro(shippng, ship.drawRec, ship.position, ship.origin, ship.rotation, RAYWHITE); //Draws player

    // Controls explosion drawing
    if (explosions.size() != 0) {
        for (int i = 0; i < (int) explosions.size(); i++) {
            DrawTexturePro(explosiontext, explosions[i].drawRec, explosions[i].position, explosions[i].origin, explosions[i].rotation, RAYWHITE);
        }
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
        for (int i = 0; i < (int)lasers.size(); i++) {
            DrawTexturePro(laser, lasers[i].drawRec, lasers[i].position, lasers[i].origin, lasers[i].rotation, RAYWHITE);
        }
    } 

    // Draws FPS on screen
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

// Checks laser's collision with all enemies & player
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

    // TODO: Fix player collision hitbox.
    if (CheckCollisionRecs(lasers[index].position, ship.position) && lasers[index].name != ship.name) {
        Object explosion = explodeanim(explosiontext, ship.position, scale);
        explosions.push_back(explosion);
        return true;
    }
    return false;
}

void MoveRandomly(int index, float axis) {
    if (axis == 0.0f) {
        float direction = RandomNum(0, 1); 
        float newx;
        if (direction == 0) {
            newx = (-1 * ENEMYSSPEED);
        } else {
            newx = ENEMYSSPEED;
        }
        if ((enemies[index].position.x + newx) > (GetScreenWidth() - (enemies[index].position.width/2.0f)) || (enemies[index].position.x + newx) < (enemies[index].position.width/2.0f)) {
            cout << "False" << endl;
            MoveRandomly(index, axis);
        }
       
        enemies[index].position.x += newx;
    } else {
        float direction = RandomNum(0, 1);
        float newy;
        if (direction == 0) {
            newy = (-1 * ENEMYSSPEED);
        } else {
            newy = ENEMYSSPEED;
        }
        if ((enemies[index].position.y + newy) < (enemies[index].position.height/2.0f) || (enemies[index].position.y + newy) > (GetScreenHeight() - (enemies[index].position.height/2.0f))) {
            cout << "False" << endl;
            MoveRandomly(index, axis);
        }
        enemies[index].position.y += newy;
    }
}