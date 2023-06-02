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

float RandomNum(int min, int max) {
    srand(time(0));
    return min + (rand() % (max + 1));
}


Object makePlayer(Texture2D ship, float scale, int ScreenWidth, int ScreenHeight) {
    Object player;
    player.drawRec = {0.0f, 0.0f, (float)((ship.width/5.0f)), (float)(ship.height/2.0f)};
    player.position = {(float)(ScreenWidth/2.0f), (float)(ScreenHeight/2.0f), (float)(ship.width/5.0f) * scale, (float)(ship.height/2.0f) * scale};
    player.origin = {(float)(player.position.width/2.0f), (float)(player.position.height/2.0f)};
    player.rotation = 0.0f;
    return player;
}

Object shootLaser(float x, float y, float rot, float textwidth, float textheight, float scale) {
    Object newLaser;
    newLaser.drawRec = {0.0f, textheight/2.0f, textwidth/2.0f, textheight/2.0f};
    newLaser.position = {x, y, textwidth/2.0f * scale, textheight/2.0f * scale};
    newLaser.origin = {textwidth/2.0f, textheight/2.0f};
    newLaser.rotation = rot;
    return newLaser;
}

Object makeSmallEnemy(Texture2D enemys, float scale) {
    Object newSmallEnemy;
    newSmallEnemy.drawRec = {0.0f, 0.0f, (float)((enemys.width/2.0f)), (float)enemys.height};
    newSmallEnemy.position = {100, GetScreenHeight()/2.0f, (float)(enemys.width/5.0f) * scale, (float)(enemys.height/2.0f) * scale};
    newSmallEnemy.origin = {(float)(newSmallEnemy.position.width/2.0f), (float)(newSmallEnemy.position.height/2.0f)};
    newSmallEnemy.rotation = 0.0f;
    newSmallEnemy.texture = enemys;
    return newSmallEnemy;
}

Object makeMediumEnemy(Texture2D enemym, float scale) {
    Object newMediumEnemy;
    newMediumEnemy.drawRec = {0.0f, 0.0f, (float)((enemym.width/2.0f)), (float)enemym.height};
    newMediumEnemy.position = {100, GetScreenHeight()/2.0f, (float)(enemym.width/5.0f) * scale, (float)(enemym.height/2.0f) * scale};
    newMediumEnemy.origin = {(float)(newMediumEnemy.position.width/2.0f), (float)(newMediumEnemy.position.height/2.0f)};
    newMediumEnemy.rotation = 0.0f;
    newMediumEnemy.texture = enemym;
    return newMediumEnemy;
}

Object makeLargeEnemy(Texture2D enemyl, float scale) {
    Object newLargeEnemy;
    newLargeEnemy.drawRec = {0.0f, 0.0f, (float)((enemyl.width/2.0f)), (float)enemyl.height};
    newLargeEnemy.position = {100, GetScreenHeight()/2.0f, (float)(enemyl.width/5.0f) * scale, (float)(enemyl.height/2.0f) * scale};
    newLargeEnemy.origin = {(float)(newLargeEnemy.position.width/2.0f), (float)(newLargeEnemy.position.height/2.0f)};
    newLargeEnemy.rotation = 0.0f;
    newLargeEnemy.texture = enemyl;
    return newLargeEnemy;
}

Object explodeanim(Texture2D explosion, Rectangle position, float scale) {
    Object newExplosion;
    newExplosion.drawRec = {0.0f, 0.0f, (float)(explosion.width/5.0f), (float) explosion.height};
    newExplosion.position = {0.0f, 0.0f, (float)(explosion.width/5.0f) * scale, (float)(explosion.height) * scale};
    newExplosion.position.x = position.x;
    newExplosion.position.y = position.y;
    newExplosion.origin = {(float)(newExplosion.position.width/2.0f), (float)(newExplosion.position.height/2.0f)};
    newExplosion.rotation = 0.0f;
    newExplosion.frame = 1;
    return newExplosion;
}

#endif