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
        int health;
        Texture2D texture;
        int frame;
};

float RandomNum(int min, int max);

//----------------------------------------------------------------------------------
// Object Functions Declarations
//----------------------------------------------------------------------------------
void MakeLaser(void);
Object makePlayer(Texture2D ship, float scale, int ScreenWidth, int ScreenHeight);
Object shootLaser(float x, float y, float rot, float textwidth, float textheight, float scale);
Object makeEnemy(Texture2D enemy, float scale, float x, float y);
Object makeLogo(Texture2D logopng, float scale);
Object makeMainMenuButton(Texture2D buttonpng, float x, float y, float scale);
Object explodeanim(Texture2D explosion, Rectangle position, float scale);

#endif