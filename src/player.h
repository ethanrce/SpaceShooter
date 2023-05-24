#include <raylib.h>

class Laser {
    public:
        Rectangle position;
        Rectangle laserRec;
        Vector2 origin;
        int rotation;
};

Laser shootLaser(float x, float y, float rot, float textwidth, float textheight, float scale) {
    Laser newLaser;
    newLaser.laserRec = {0.0f, textheight/2.0f, textwidth/2.0f, textheight/2.0f};
    newLaser.position = {x, y, textwidth/2.0f * scale, textheight/2.0f * scale};
    newLaser.origin = {textwidth/2.0f, textheight/2.0f};
    newLaser.rotation = rot;
    return newLaser;
};
