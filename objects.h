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
Object makePlayer(Texture2D ship, float scale, int ScreenWidth, int ScreenHeight);
Object shootLaser(float x, float y, float rot, float textwidth, float textheight, float scale);
Object makeSmallEnemy(Texture2D enemys, float scale);
Object makeMediumEnemy(Texture2D enemym, float scale);
Object makeLargeEnemy(Texture2D enemyl, float scale);
Object makeLogo(Texture2D logopng);
Object makeMainMenuButton(Texture2D buttonpng, float x, float y);
Object explodeanim(Texture2D explosion, Rectangle position, float scale);

#endif