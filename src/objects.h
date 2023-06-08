#ifndef OBJECTS_H
#define OBJECTS_H

#include <raylib.h>
#include <iostream>
using namespace std;

class Object {
    public:
        Rectangle drawRec;
        Rectangle position;
        Vector2 origin;
        int rotation;
        int wpindex;
        Texture2D texture;
        int frame;
        const char *name;
};

class Waypoint {
    public:
        int x;
        int y;
};

typedef enum {
    STORAGE_POSITION_HISCORE      = 0,
} StorageData;

//----------------------------------------------------------------------------------
// Saving Function Declarations
//----------------------------------------------------------------------------------
int GetHighScore(void);

//----------------------------------------------------------------------------------
// Object Function Declarations
//----------------------------------------------------------------------------------

Object makePlayer(Texture2D ship, float scale, int ScreenWidth, int ScreenHeight);
Object shootLaser(float x, float y, float rot, float textwidth, float textheight, float scale, const char *obj);
Object makeEnemy(Texture2D enemy, float scale, float x, float y, const char *obj);
Object makeLogo(Texture2D logopng, float scale);
Object makeMainMenuButton(Texture2D buttonpng, float x, float y, float scale);
Object explodeanim(Texture2D explosion, Rectangle position, float scale);
Waypoint makeWaypoint(int x, int y);
void MakeLaser(Object obj);
void makeWaypoints(int wpnum);

//----------------------------------------------------------------------------------
// Object Movement/Animation Function Declarations
//----------------------------------------------------------------------------------
void Animations(void);
void Movement(void);
void MoveRandomly(int index, float axis);
void MoveEnemy(int index);
void RotateEnemy(int index);

//----------------------------------------------------------------------------------
// Gameplay Functions Declarations
//----------------------------------------------------------------------------------
float RandomNum(int min, int max);
void SpawnWave(int wavenum);
bool checkCollisions(int index);
void DrawWave(void);
bool FinishTransition(void);
bool FinishWave(void);
void DrawEndGame(void);
#endif