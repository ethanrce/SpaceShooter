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
#define SHIPSPEED 5.0f
#define SHIPROTATION 4.0f
#define LASERSPEED 5.0f
#define LASERCOOLDOWN 1.0f // Seconds between laser shots
#define ENEMYSSPEED 1.0f
#define ENEMYSROTATION 1.5f
#define ENEMYLASERCOOLDOWN 3.0f // Seconds between enemy laser shots
#define SCREENHEIGHT 800
#define SCREENWIDTH 800
#define WAVECOOLDOWN 5.0f

// Game texture/Object variables
Texture2D shippng;
Object ship;
Texture2D laser;
vector<Object> lasers;
vector<Object> enemies;
Texture explosiontext;
vector<Object> explosions;
Texture2D enemys;
Texture2D enemym;
Texture2D enemyl;
vector<Waypoint> waypoints;
GamePhase CurrentPhase;

float scale; // Game texture scaling
float shipscale;
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
bool EndGame;
int wave;
Font alphabeta;
bool TransitionOver;
int wavecounter;

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
    CurrentPhase = TRANSITION; // TODO: Work on wave/wavepoint system
    EndGame = false;
    wave = 1;
    alphabeta = LoadFont("assets/Fonts/alpha_beta.png");
    TransitionOver = false;
    wavecounter = 0;
}

void LoadTextures(void) {
    shippng = LoadTexture("assets/spritesheets/ship.png"); 
    // TODO: Need to change scale after fullscreen is enabled/disabled.
    scale = (0.1/((shippng.width/5.0f)/GetScreenWidth())); // Character's width should be 10% of screen. 
    shipscale = (0.07/((shippng.width/5.0f)/GetScreenWidth()));
    ship = makePlayer(shippng, shipscale, GetScreenWidth(), GetScreenHeight());

    explosiontext = LoadTexture("assets/spritesheets/explosion.png");
   
    laser = LoadTexture("assets/spritesheets/laser-bolts.png");

    enemys = LoadTexture("assets/spritesheets/enemy-small.png");
    enemym = LoadTexture("assets/spritesheets/enemy-medium.png");
    enemyl = LoadTexture("assets/spritesheets/enemy-big.png");
}

void UpdateGame(void) {
    Animations(); 
    Movement();
    switch(CurrentPhase) {
        case TRANSITION: {
            if (FinishTransition()) {
                CurrentPhase = INWAVE;
            }
        } break;
        case INWAVE: {
            SpawnWave(wave);
        } break;    
    }   
}

void DrawGame(void) {
    DrawTexturePro(shippng, ship.drawRec, ship.position, ship.origin, ship.rotation, RAYWHITE); //Draws player
    DrawFPS(10, 10); // Draws FPS 
    switch(CurrentPhase) {
        case TRANSITION: {
            DrawWave();
        } break;
        case INWAVE: {
            // Controls explosion drawing
            if (explosions.size() != 0) {
                for (int i = 0; i < (int) explosions.size(); i++) {
                    DrawTexturePro(explosiontext, explosions[i].drawRec, explosions[i].position, explosions[i].origin, explosions[i].rotation, RAYWHITE);
                }
            }

            // Controls enemy drawing
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
        } break;    
    }

}

void DrawWave(void) {
    const char *msg = ("Wave: " + std::to_string(wave)).c_str();
    // TODO: Account for fullscreen
    Vector2 wavetextsize = MeasureTextEx(alphabeta, msg, (float)(alphabeta.baseSize * scale), (float) 4.0f);
    DrawTextEx(alphabeta, msg, (Vector2){((GetScreenWidth()/2.0f) - (wavetextsize.x/2.0f)), (GetScreenHeight()/2.0f)}, alphabeta.baseSize*scale, 4.0f, GOLD);
    wavecounter ++;
    if (wavecounter >= ((float)(fps/(1.0f/WAVECOOLDOWN)))) {
        TransitionOver = true;
        wavecounter = 0;
    }
}

bool FinishTransition(void) {
    return TransitionOver;
}

void UnloadGame(void) {
    UnloadTexture(shippng);
    UnloadTexture(laser);
    UnloadTexture(enemys);
    UnloadTexture(enemym);
    UnloadTexture(enemyl);
    UnloadFont(alphabeta);
}

int FinishGame(void) {
    return EndGame;
}

void SpawnWave(int wavenum) {
    makeWaypoints(wavenum + 1);
    if (wavenum == 1) {
        float axischoice = RandomNum(0, 4);
        if (axischoice == 0) {
            Object enemy = makeEnemy(enemyl, scale, RandomNum(0, GetScreenWidth()), 0.0f, "enemy");
            enemies.push_back(enemy);
        } else if (axischoice == 1) {
            Object enemy = makeEnemy(enemyl, scale, GetScreenWidth(), RandomNum(0, GetScreenHeight()), "enemy");
            enemies.push_back(enemy);
        } else if (axischoice == 2) {
            Object enemy = makeEnemy(enemyl, scale, RandomNum(0, GetScreenWidth()), GetScreenHeight(), "enemy");
            enemies.push_back(enemy);
        } else {
            Object enemy = makeEnemy(enemyl, scale, 0.0f, RandomNum(0, GetScreenHeight()), "enemy");
            enemies.push_back(enemy);
        }
    }
}

void makeWaypoints(int wavepoints) {
    for (int i = 0; i < wavepoints; i++) {
        Waypoint wp = makeWaypoint((int)RandomNum(0, GetScreenWidth()), (int)RandomNum(0, GetScreenHeight()));
        waypoints.push_back(wp);
    }
}
// Constructs a new laser if player can shoot.
// TODO: Make laser shoot from front of ship instead of middle.
void MakeLaser(Object obj) {
    Object newLaser = shootLaser(obj.position.x, obj.position.y, obj.rotation, laser.width, laser.height, scale/2.0f, obj.name);
    lasers.push_back(newLaser);
}

void Movement(void) {
    // TODO: Need to fix borders during window resizing in main menu. (depends on itch.io structure)
    // TODO: Allow for rotation speed slider in main menu
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        if (ship.position.x < (GetScreenWidth() - (ship.position.width/2.0f))) {
            ship.drawRec.y = (shippng.height/2.0f);
            ship.rotation += SHIPROTATION;
        }
    }
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        if ((int)ship.position.x >= (ship.position.width/2.0f)) {
            ship.drawRec.y = (shippng.height/2.0f);
            ship.rotation -= SHIPROTATION;
        }
    }
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
        ship.drawRec.y = shippng.height/2.0f;
        ship.position.x += SHIPSPEED * sinf(ship.rotation * (PI / 180.0f));
        ship.position.y -= SHIPSPEED * cosf(ship.rotation * (PI / 180.0f));        
    }
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)){
        ship.drawRec.y = shippng.height/2.0f;
        ship.position.x -= SHIPSPEED * sinf(ship.rotation * (PI / 180.0f));
        ship.position.y += SHIPSPEED * cosf(ship.rotation * (PI / 180.0f));
    }   

    // Controls game world borders
    // TODO: Change for fullscreen
    if ((int) ship.position.y <= (int)(ship.position.height/2.0f)) {
            ship.position.y = (ship.position.height/2.0f);
        }
    if ((int) ship.position.y >= (int)(GetScreenHeight() - (ship.position.height/2.0f))) {
        ship.position.y = (GetScreenHeight() - (ship.position.height/2.0f));
    }
    if ((int) ship.position.x <= (int)(ship.position.width/2.0f)) {
        ship.position.x = (ship.position.width/2.0f);
    }
    if ((int) ship.position.x >= (int)(GetScreenWidth() - (ship.position.width/2.0f))) {
        ship.position.x = (GetScreenWidth() - (ship.position.width/2.0f));
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
        MoveEnemy(i);
        RotateEnemy(i);   
    }
}
// Checks laser's collision with all enemies & player
bool checkCollisions(int index) {
    Vector2 lasercenter = {lasers[index].position.x, lasers[index].position.y};
    for (int i = 0; i < (int) enemies.size(); i++) {
        Vector2 enemycenter = {enemies[i].position.x, enemies[i].position.y};
        if (CheckCollisionCircles(lasercenter, (lasers[index].position.width/2.0f), enemycenter, (enemies[i].position.width/2.0f)) && lasers[index].name != enemies[i].name) {
            Object explosion = explodeanim(explosiontext, enemies[i].position, scale);
            explosions.push_back(explosion);
            enemies.erase(enemies.begin() + i);
            if (enemies.size() != 0) {
                i --;    
            }
            return true;
        }        
    }

    Vector2 playercenter = {ship.position.x, ship.position.y};
    if (CheckCollisionCircles(lasercenter, (lasers[index].position.width/2.0f), playercenter, (ship.position.width/2.0f)) && lasers[index].name != ship.name) {
        Object explosion = explodeanim(explosiontext, ship.position, scale);
        explosions.push_back(explosion);
        return true;
    }
    return false;
}

void Animations(void) {
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
}

void MoveEnemy(int index) {
    Waypoint currwp = waypoints[enemies[index].wpindex];

    if (enemies[index].position.x < currwp.x) {
        enemies[index].position.x += ENEMYSSPEED;
    } else if (enemies[index].position.x > currwp.x) {
        enemies[index].position.x -= ENEMYSSPEED;
    }

    if (enemies[index].position.y < currwp.y) {
        enemies[index].position.y += ENEMYSSPEED;
    } else if (enemies[index].position.y > currwp.y) {
        enemies[index].position.y -= ENEMYSSPEED;
    }

    if (enemies[index].position.x == currwp.x && enemies[index].position.y == currwp.y) {
        enemies[index].wpindex ++;
        if (enemies[index].wpindex >= (int) waypoints.size()) {
            enemies[index].wpindex = 0;
        }
    }
}

void RotateEnemy(int index) {
    float rotationAngle = ((std::atan2(ship.position.y - enemies[index].position.y, ship.position.x - enemies[index].position.x) * 180.0)/PI);
        float rotationDiff = rotationAngle - (enemies[index].rotation + 90.0f);
         while (rotationDiff > 180.0f) {
            rotationDiff -= 360.0f;
         }  
        while (rotationDiff < -180.0f) {
            rotationDiff += 360.0f;
        }
        float maxRotationChange = ENEMYSROTATION;
        float rotationChange = Clamp(rotationDiff, -maxRotationChange, maxRotationChange);
        enemies[index].rotation += rotationChange;
}

void SpawnEnemy(const char *type) {

}