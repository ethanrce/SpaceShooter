#include "raylib.h"
#include "raymath.h"
#include "screens.h"
#include <iostream>
using std::cout;
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
Font setback;
const char *message;
int MMhighscore;
Font romulus2;
Sound buttonsound;

// TODO: Implement an options button
void InitMainMenu(void) {
    // TODO: Draw  fullscreen button on MainMenu.
    MMscale = CheckWidth()/800; // Want logos to be 10% of screen
    GameLogo = LoadTexture("assets/MainMenu/GameLogo.png");
    Logo = makeLogo(GameLogo, MMscale);

    PlayButton = LoadTexture("assets/MainMenu/PlayButton.png");
    Play = makeMainMenuButton(PlayButton, (float)((CheckWidth()/2.0f)), (float)((CheckHeight()/4.0f)), MMscale);

    buttonsound = LoadSound("assets/Audio/PlayButtonfx.wav");
    mousePoint = {0.0f, 0.0f};

    btnAction = false;
    btnState = 0;
    
    EndMainMenu = false;
    setback = LoadFont("assets/Fonts/setback.png");
    message = "created by @Ethom";
    MMhighscore = GetHighScore();
    romulus2 = LoadFont("assets/Fonts/romulus.png");
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
        PlaySound(buttonsound);
    }
    Play.drawRec.y = (float)((Play.texture.height/3.0f) * btnState);
}

void DrawMainMenu(void) {
    DrawTexturePro(GameLogo, Logo.drawRec, Logo.position, Logo.origin, Logo.rotation, RAYWHITE);
    DrawTexturePro(PlayButton, Play.drawRec, Play.position, Play.origin, Play.rotation, RAYWHITE);
    // DrawLine((int)((CheckWidth()/2.0f)-3), 0, (int)((CheckWidth()/2.0f)+3), (int)(CheckHeight()), RED);
    Vector2 textSize = MeasureTextEx(setback, message, (float)(setback.baseSize * MMscale * 3.0f), (float) 4.0f);
    DrawTextEx(setback, message, (Vector2) {((CheckWidth()/2.0f) - (textSize.x/2.0f)), ((Logo.position.y + Play.position.y)/2.0f)}, (float)(setback.baseSize * MMscale * 3.0f), (float) 4.0f, MAROON);
    Vector2 textSize2 = MeasureTextEx(romulus2, ("HighScore: " + std::to_string(MMhighscore)).c_str(), (float)(romulus2.baseSize * MMscale * 3.0f), (float) 4.0f);
    DrawTextEx(romulus2, ("HighScore: " + std::to_string(MMhighscore)).c_str(), (Vector2) {((CheckWidth()/2.0f) - (textSize2.x/2.0f)), (CheckHeight()/2.0f) + (CheckHeight()/6.0f)}, (float)(romulus2.baseSize * MMscale * 3.0f), (float) 4.0f, PINK);
    Vector2 textSize3 = MeasureTextEx(romulus2, "Press (F) to Shoot", (float)(romulus2.baseSize * MMscale * 3.0f), (float) 4.0f);
    DrawTextEx(romulus2, "Press (F) to Shoot", (Vector2) {((CheckWidth()/2.0f) - (textSize3.x/2.0f)), (CheckHeight()/2.0f) + (CheckHeight()/4.0f)}, (float)(romulus2.baseSize * MMscale * 3.0f), (float) 4.0f, PINK);
    //TODO: Draw fullscreen button)
}

void UnloadMainMenu(void) {
    UnloadTexture(GameLogo);
    UnloadTexture(PlayButton);
    UnloadFont(setback);
    UnloadFont(romulus2);
    UnloadSound(buttonsound);
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