#include "raylib.h"
#include "raymath.h"
#include "screens.h"
#include <iostream>
using namespace std;

Texture2D MMbackground;
NPatchInfo backgroundinfo;
Texture2D GameLogo;
Rectangle LogoRec;
Rectangle LogoPos;
Vector2 LogoOrigin;

void InitMainMenu(Texture2D backgroundpng) {
    MMbackground = backgroundpng;
    backgroundinfo = {(Rectangle) {0.0f, 0.0f, (float) MMbackground.width, (float) MMbackground.height}, 0, 0, 0, 0, NPATCH_NINE_PATCH};

    GameLogo = LoadTexture("assets/MainMenu/GameLogo.png");
    LogoRec = {0.0f, 0.0f, (float)(GameLogo.width), (float)(GameLogo.height)};
    LogoPos = {(float)(GetScreenWidth()/2.0f), (float)(GetScreenHeight()/6.0f), (float)(GameLogo.width * 0.8), (float)(GameLogo.height * 0.7)};
    LogoOrigin = {(float)(LogoPos.width/2.0f), (float)(LogoPos.height/2.0f)};
}

void UpdateMainMenu(void) {

}

void DrawMainMenu(void) {
    DrawTextureNPatch(MMbackground, backgroundinfo, (Rectangle) {0.0f, 0.0f, (float) GetScreenWidth(), (float) GetScreenHeight()}, Vector2Zero(), 0.0f, RAYWHITE); // Draw's background
    DrawTexturePro(GameLogo, LogoRec, LogoPos, LogoOrigin, 0.0f, RAYWHITE);

    //TODO: Draw play & settings buttons
}

void UnloadMainMenu(void) {

}

int FinishMainMenu(void) {
    return 0;
}