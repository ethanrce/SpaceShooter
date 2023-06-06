#include "raylib.h"
#include "raymath.h"
#include "screens.h"
#include <iostream>
#include "objects.h"

Texture2D GameLogo;
Object Logo;
Texture2D PlayButton;
Object Play;
Vector2 mousePoint; // Gets mouse's position for button collision
bool btnAction;
int btnState;
bool EndMainMenu; 
float MMscale; //Logos should always be 

void InitMainMenu(void) {
    // TODO: Need to change scale after fullscreen is enabled/disabled
    MMscale = 1.5f; 
    GameLogo = LoadTexture("assets/MainMenu/GameLogo.png");
    Logo = makeLogo(GameLogo, MMscale);

    PlayButton = LoadTexture("assets/MainMenu/PlayButton.png");
    Play = makeMainMenuButton(PlayButton, (float)((GetScreenWidth()/2.0f)), (float)((GetScreenHeight()/4.0f)), MMscale);

    mousePoint = {0.0f, 0.0f};

    btnAction = false;
    btnState = 0;
    
    EndMainMenu = false;
    
}

void UpdateMainMenu(void) {
    btnAction = false;
    // Check button state
    if (CheckButtonHover()) {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            btnState = 2;
            } else {
            btnState = 1;
            }
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            btnAction = true;
            } 
    } else {
        btnState = 0;
    }

    if (btnAction)
    {
        EndMainMenu = true;
        //    PlaySound(fxButton);
    }
    Play.drawRec.y = (float)((Play.texture.height/3.0f) * btnState);
}

void DrawMainMenu(void) {
    DrawTexturePro(GameLogo, Logo.drawRec, Logo.position, Logo.origin, Logo.rotation, RAYWHITE);
    DrawTexturePro(PlayButton, Play.drawRec, Play.position, Play.origin, Play.rotation, RAYWHITE);
    //TODO: Draw fullscreen button
}

void UnloadMainMenu(void) {
    UnloadTexture(GameLogo);
    UnloadTexture(PlayButton);
}

bool FinishMainMenu(void) {
    return EndMainMenu;
}

// Checks if the mouse is hovering over buttons.
bool CheckButtonHover(void) {
    mousePoint = GetMousePosition();
    if (CheckCollisionPointRec(mousePoint, Play.position)) {
        return true;
    }
    return false;
}