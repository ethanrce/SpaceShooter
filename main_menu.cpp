#include "raylib.h"
#include "raymath.h"
#include "screens.h"
#include <iostream>
using namespace std;
#include "objects.h"
 
Texture2D MMbackground;
NPatchInfo backgroundinfo;
Texture2D GameLogo;
Object Logo;
Texture2D PlayButton;
Object Play;
Vector2 mousePoint; // Gets mouse's position for button collision

void InitMainMenu(Texture2D backgroundpng) {
    MMbackground = backgroundpng;
    backgroundinfo = {(Rectangle) {0.0f, 0.0f, (float) MMbackground.width, (float) MMbackground.height}, 0, 0, 0, 0, NPATCH_NINE_PATCH};
    
    GameLogo = LoadTexture("assets/MainMenu/GameLogo.png");
    Logo = makeLogo(GameLogo);

    PlayButton = LoadTexture("assets/MainMenu/PlayButton.png");
    Play = makeMainMenuButton(PlayButton, (float)((GetScreenWidth()/2.0f)), (float)((GetScreenHeight()/2.0f)));

    mousePoint = {0.0f, 0.0f};
}

void UpdateMainMenu(int btnState) {
    Play.drawRec.y = (float)((Play.texture.height/3.0f) * btnState);
}

void DrawMainMenu(void) {
    DrawTextureNPatch(MMbackground, backgroundinfo, (Rectangle) {0.0f, 0.0f, (float) GetScreenWidth(), (float) GetScreenHeight()}, Vector2Zero(), 0.0f, RAYWHITE); // Draw's background
    DrawTexturePro(GameLogo, Logo.drawRec, Logo.position, Logo.origin, Logo.rotation, RAYWHITE);
    DrawTexturePro(PlayButton, Play.drawRec, Play.position, Play.origin, Play.rotation, RAYWHITE);
    //TODO: Draw settings button
}

void UnloadMainMenu(void) {
    UnloadTexture(MMbackground);
    UnloadTexture(GameLogo);
    UnloadTexture(PlayButton);
}

int FinishMainMenu(void) {
    return 0;
}

// Checks if the mouse is hovering over buttons.
bool CheckButtonHover(void) {
    mousePoint = GetMousePosition();
    if (CheckCollisionPointRec(mousePoint, Play.position)) {
        return true;
    }
    return false;
}