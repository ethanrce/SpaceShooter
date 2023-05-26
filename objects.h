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

Object makePlayer(Texture2D ship, float scale, int ScreenWidth, int ScreenHeight) {
    Object player;
    player.drawRec = {0.0f, 0.0f, (float)((ship.width/5.0f)), (float)(ship.height/2.0f)};
    player.position = {(float)(ScreenWidth/2.0f), (float)(ScreenHeight/2.0f), (float)(ship.width/5.0f) * scale, (float)(ship.height/2.0f) * scale};
    player.origin = {(float)(player.position.width/2.0f), (float)(player.position.height/2.0f)};
    player.rotation = 0.0f;
    return player;
};

Object shootLaser(float x, float y, float rot, float textwidth, float textheight, float scale) {
    Object newLaser;
    newLaser.drawRec = {0.0f, textheight/2.0f, textwidth/2.0f, textheight/2.0f};
    newLaser.position = {x, y, textwidth/2.0f * scale, textheight/2.0f * scale};
    newLaser.origin = {textwidth/2.0f, textheight/2.0f};
    newLaser.rotation = rot;
    return newLaser;
};

Object makeSmallEnemy(Texture2D enemys, float scale) {
    Object newSmallEnemy;
    newSmallEnemy.drawRec = {0.0f, 0.0f, (float)((enemys.width/2.0f)), (float)enemys.height};
    newSmallEnemy.position = {20, GetScreenHeight()/2.0f, (float)(enemys.width/5.0f) * scale, (float)(enemys.height/2.0f) * scale};
    newSmallEnemy.origin = {(float)(newSmallEnemy.position.width/2.0f), (float)(newSmallEnemy.position.height/2.0f)};
    newSmallEnemy.texture = enemys;
    return newSmallEnemy;
};

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
};

/*
Object makeMediumEnemy(Texture2D enemym) {
    Object newSmallEnemy;
    Rectangle esRec = {0.0f, 0.0f, (float)((enemys.width/2.0f)), (float)enemys.height};
    Rectangle esposition = {20, GetScreenHeight()/2.0f, (float)(enemys.width/5.0f) * scalerate, (float)(enemys.height/2.0f) * scalerate};
    Vector2 esorigin = {(float)(esposition.width/2.0f), (float)(esposition.height/2.0f)};
    return newSmallEnemy;
};

Object makeLargeEnemy(Texture2D enemyl) {
    Object newSmallEnemy;
    Rectangle esRec = {0.0f, 0.0f, (float)((enemys.width/2.0f)), (float)enemys.height};
    Rectangle esposition = {20, GetScreenHeight()/2.0f, (float)(enemys.width/5.0f) * scalerate, (float)(enemys.height/2.0f) * scalerate};
    Vector2 esorigin = {(float)(esposition.width/2.0f), (float)(esposition.height/2.0f)};
    return newSmallEnemy;
};
*/
