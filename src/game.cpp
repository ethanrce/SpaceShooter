#include <raylib.h>
#include <raymath.h>
#include <fstream>
#include <iostream>
using std::cout;
#include "objects.h"
#include <vector>
#include "screens.h"
#include <stdlib.h>         // Required for: calloc(), free()

// static variables
#define FRAMESPEED 8
//TODO: Speed & rotation should change depending on the window size.
#define SHIPSPEED 5.0f
#define SHIPROTATION 4.0f
#define LASERSPEED 6.5f
#define LASERCOOLDOWN 1.0f // Seconds between laser shots
#define ENEMYSSPEED 1.5f
#define ENEMYSROTATION 1.5f
#define ENEMYLASERCOOLDOWN 3.0f // Seconds between enemy laser shots
#define SCREENHEIGHT 800
#define SCREENWIDTH 800
#define WAVECOOLDOWN 5.0f
#define TRANSITIONCOOLDOWN 3.0f
#define ENDGAMECOOLDOWN 6.0f
#define STORAGE_DATA_FILE "storage.data"

static bool SaveStorageValue(unsigned int position, int value);
static int LoadStorageValue(unsigned int position);

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
bool WaveOver;
int transitioncounter;
int score;
Font romulus;
int highscore;


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
    CurrentPhase = TRANSITION;
    EndGame = false;
    wave = 1;
    alphabeta = LoadFont("assets/Fonts/alpha_beta.png");
    TransitionOver = false;
    wavecounter = 0;
    WaveOver = false;
    transitioncounter = 0;
    score = 0;
    romulus = LoadFont("assets/Fonts/romulus.png");
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
                TransitionOver = false;
                SpawnWave(wave);
                WaveOver = false;
                CurrentPhase = INWAVE;
            }
        } break;
        case INWAVE: {
            if (FinishWave()) {
                wave ++;
                CurrentPhase = TRANSITION;
            }
        } break;    
    }   
}
// TODO: Keeping track of highscore (Drawing on MainMenu & when occurs)
void DrawGame(void) {
    if (!EndGame) {
        DrawTexturePro(shippng, ship.drawRec, ship.position, ship.origin, ship.rotation, RAYWHITE); //Draws player
    }
    const char *wavemsg = ("Wave: " + std::to_string(wave)).c_str();
    Vector2 wavetxtsize = MeasureTextEx(romulus, wavemsg, (float)(romulus.baseSize * (scale/2.0f)), (float) 4.0f);
    DrawTextEx(romulus, wavemsg, (Vector2){(GetScreenWidth() - wavetxtsize.x) - 10, 10}, romulus.baseSize*(scale/2.0f), 4.0f, GOLD);
    const char *scoremsg = ("Score: " + std::to_string(score)).c_str();
    Vector2 scoretextsize = MeasureTextEx(romulus, scoremsg, (float)(romulus.baseSize * (scale/2.0f)), (float) 4.0f);
    DrawTextEx(romulus, scoremsg, (Vector2){((GetScreenWidth()/2.0f) - (scoretextsize.x/2.0f)), 10}, romulus.baseSize*(scale/2.0f), 4.0f, GOLD);
    // Controls explosion drawing
    if (explosions.size() != 0) {
        for (int i = 0; i < (int) explosions.size(); i++) {
            DrawTexturePro(explosiontext, explosions[i].drawRec, explosions[i].position, explosions[i].origin, explosions[i].rotation, RAYWHITE);
        }
    }   
    DrawFPS(10, 10); // Draws FPS
    switch(CurrentPhase) {
        case TRANSITION: {
            if (!EndGame) {
                DrawWave();
            } else {
                DrawEndGame(); //Draw Game End Screen
            }
        } break;
        case INWAVE: {
            // Controls enemy drawing
            if (enemies.size() != 0) {
                for (int i = 0; i < (int) enemies.size(); i++) {
                    DrawTexturePro(enemies[i].texture, enemies[i].drawRec, enemies[i].position, enemies[i].origin, enemies[i].rotation, RAYWHITE);
                }
            } else {
                WaveOver = true;
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
    DrawTextEx(alphabeta, msg, (Vector2){((GetScreenWidth()/2.0f) - (wavetextsize.x/2.0f)), (GetScreenHeight()/2.0f)}, alphabeta.baseSize*scale, 4.0f, DARKPURPLE);
    wavecounter ++;
    if (wavecounter >= ((float)(fps/(1.0f/WAVECOOLDOWN)))) {
        TransitionOver = true;
        wavecounter = 0;
    }
}

void DrawEndGame() {
    // TODO: Account for fullscreen
    Vector2 txtsize1 = MeasureTextEx(alphabeta, "You Died", (float)(alphabeta.baseSize * scale), (float) 4.0f);
    DrawTextEx(alphabeta, "You Died", (Vector2){((GetScreenWidth()/2.0f) - (txtsize1.x/2.0f)), ((GetScreenHeight()/2.0f) - 100)}, alphabeta.baseSize*scale, 4.0f, RED);
    Vector2 txtsize2 = MeasureTextEx(alphabeta, ("Score: " + std::to_string(score)).c_str(), (float)(alphabeta.baseSize * scale), (float) 4.0f);
    DrawTextEx(alphabeta, ("Score: " + std::to_string(score)).c_str(), (Vector2){((GetScreenWidth()/2.0f) - (txtsize2.x/2.0f)), (GetScreenHeight()/2.0f)}, alphabeta.baseSize*scale, 4.0f, GOLD);
    if (score > highscore) {
        Vector2 txtsize3 = MeasureTextEx(alphabeta, "You Got A New Highscore!", (float)(alphabeta.baseSize * scale), (float) 4.0f);
        DrawTextEx(alphabeta, "You Got A New Highscore!", (Vector2){((GetScreenWidth()/2.0f) - (txtsize3.x/2.0f)), ((GetScreenHeight()/2.0f) + 100)}, alphabeta.baseSize*scale, 4.0f, GOLD);
    }
}

bool FinishTransition(void) {
    return TransitionOver;
}

bool FinishWave(void) {
    if (WaveOver) {
        transitioncounter ++;
        if (transitioncounter >= (fps/(1.0/TRANSITIONCOOLDOWN))) {
            transitioncounter = 0;
            return true;
        }
    }
    return false;
}

// TODO: Make game ending implementation when player is shot
int FinishGame(void) {
    if (EndGame) {
        CurrentPhase = TRANSITION;
        transitioncounter ++;
        if (transitioncounter >= (fps/(1.0/ENDGAMECOOLDOWN))) {
            if (score > highscore) {
                SaveStorageValue(STORAGE_POSITION_HISCORE, score);
            }
            return true;
        }
    }
    return false;
}

int GetHighScore(void) {
    return highscore;
}

void UnloadGame(void) {
    UnloadTexture(shippng);
    UnloadTexture(laser);
    UnloadTexture(enemys);
    UnloadTexture(enemym);
    UnloadTexture(enemyl);
    UnloadFont(alphabeta);
    UnloadFont(romulus);
    enemies.clear();
    lasers.clear();
    explosions.clear();
    waypoints.clear();
}

// Clean up wave spawning system && make it fully modular/randomized
void SpawnWave(int wavenum) {
    makeWaypoints(wavenum + 1);
    for (int i = 0; i < wavenum; i++) {
        Object enemy;
        float axischoice = RandomNum(0, 3);
        float type = RandomNum(0, 2);
        Vector2 enemypos;
        if (axischoice == 0) {
            enemypos = {RandomNum(0, GetScreenWidth()), 0.0f};
        } else if (axischoice == 1) {
            enemypos = {(float)GetScreenWidth(), RandomNum(0, GetScreenHeight())};
        } else if (axischoice == 2) {
            enemypos = {RandomNum(0, GetScreenWidth()), (float)GetScreenHeight()};
        } else {
            enemypos = {0.0f, RandomNum(0, GetScreenHeight())};
        }
        if (type == 0) {
            enemy = makeEnemy(enemys, scale, enemypos.x, enemypos.y, "enemys");
        } else if (type == 1) {
            enemy = makeEnemy(enemym, scale, enemypos.x, enemypos.y, "enemym");
        } else {
            enemy = makeEnemy(enemyl, scale, enemypos.x, enemypos.y, "enemyl");
        } 
        enemy.wpindex = i;
        enemies.push_back(enemy);  
    }
}

void makeWaypoints(int wps) {
    if (waypoints.size() > 0) {
        waypoints.clear();
    }
    for (int i = 0; i < wps; i++) {
        Waypoint wp = makeWaypoint((int)RandomNum(20, GetScreenWidth() - 20), (int)RandomNum(20, GetScreenHeight() - 20));
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
    if (IsKeyPressed(KEY_F) && canShoot && CurrentPhase == INWAVE && !EndGame) {
        canShoot = false;
        lasershootcounter = 0;
        MakeLaser(ship);
    }
     
    // Controls enemy laser shooting cooldown and updating position
    for (int i = 0; i < (int) enemies.size(); i++) {
        enemies[i].frame ++;
        if (enemies[i].frame >= (float)(fps/(1.0f/ENEMYLASERCOOLDOWN)) && !EndGame) {
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
        if (CheckCollisionCircles(lasercenter, (lasers[index].position.width/2.0f), enemycenter, (enemies[i].position.width/2.0f)) && lasers[index].name != std::string("enemy")) {
            if (std::string(enemies[i].name) == "enemyl") {
                score += 100;
            } else if (std::string(enemies[i].name) == "enemym") {
                score += 150;
            } else if (std::string(enemies[i].name) == "enemys") {
                score += 200;
            }
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
    if (CheckCollisionCircles(lasercenter, (lasers[index].position.width/2.0f), playercenter, (ship.position.width/2.0f)) && lasers[index].name != ship.name && !EndGame) {
        highscore = LoadStorageValue(STORAGE_POSITION_HISCORE);
        EndGame = true;
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

    if ( (abs((int)(enemies[index].position.x) - currwp.x)) < 2 && abs(((int)(enemies[index].position.y) - currwp.y)) < 2)  {
        int wpnum = enemies[index].wpindex;
        while (enemies[index].wpindex == wpnum) {
            enemies[index].wpindex = RandomNum(0, (int) waypoints.size() - 1);
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


bool SaveStorageValue(unsigned int position, int value)
{
    bool success = false;
    unsigned int dataSize = 0;
    unsigned int newDataSize = 0;
    unsigned char *fileData = LoadFileData(STORAGE_DATA_FILE, &dataSize);
    unsigned char *newFileData = NULL;

    if (fileData != NULL)
    {
        if (dataSize <= (position*sizeof(int)))
        {
            // Increase data size up to position and store value
            newDataSize = (position + 1)*sizeof(int);
            newFileData = (unsigned char *)RL_REALLOC(fileData, newDataSize);

            if (newFileData != NULL)
            {
                // RL_REALLOC succeded
                int *dataPtr = (int *)newFileData;
                dataPtr[position] = value;
            }
            else
            {
                // RL_REALLOC failed
                TraceLog(LOG_WARNING, "FILEIO: [%s] Failed to realloc data (%u), position in bytes (%u) bigger than actual file size", STORAGE_DATA_FILE, dataSize, position*sizeof(int));

                // We store the old size of the file
                newFileData = fileData;
                newDataSize = dataSize;
            }
        }
        else
        {
            // Store the old size of the file
            newFileData = fileData;
            newDataSize = dataSize;

            // Replace value on selected position
            int *dataPtr = (int *)newFileData;
            dataPtr[position] = value;
        }

        success = SaveFileData(STORAGE_DATA_FILE, newFileData, newDataSize);
        RL_FREE(newFileData);

        TraceLog(LOG_INFO, "FILEIO: [%s] Saved storage value: %i", STORAGE_DATA_FILE, value);
    }
    else
    {
        TraceLog(LOG_INFO, "FILEIO: [%s] File created successfully", STORAGE_DATA_FILE);

        dataSize = (position + 1)*sizeof(int);
        fileData = (unsigned char *)RL_MALLOC(dataSize);
        int *dataPtr = (int *)fileData;
        dataPtr[position] = value;

        success = SaveFileData(STORAGE_DATA_FILE, fileData, dataSize);
        UnloadFileData(fileData);

        TraceLog(LOG_INFO, "FILEIO: [%s] Saved storage value: %i", STORAGE_DATA_FILE, value);
    }

    return success;
}

// Load integer value from storage file (from defined position)
// NOTE: If requested position could not be found, value 0 is returned
int LoadStorageValue(unsigned int position)
{
    int value = 0;
    unsigned int dataSize = 0;
    unsigned char *fileData = LoadFileData(STORAGE_DATA_FILE, &dataSize);

    if (fileData != NULL)
    {
        if (dataSize < (position*4)) TraceLog(LOG_WARNING, "FILEIO: [%s] Failed to find storage position: %i", STORAGE_DATA_FILE, position);
        else
        {
            int *dataPtr = (int *)fileData;
            value = dataPtr[position];
        }

        UnloadFileData(fileData);

        TraceLog(LOG_INFO, "FILEIO: [%s] Loaded storage value: %i", STORAGE_DATA_FILE, value);
    }

    return value;
}