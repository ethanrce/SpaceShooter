#include <raylib.h>
#include <raymath.h>
#include <iostream>
using std::cout;
#include "objects.h"
#include "screens.h"

#define SCREENHEIGHT 1200
#define SCREENWIDTH 1200

GameScreen currentScreen;
Texture2D background;
NPatchInfo backgroundinfo;
int display;
int fps;

int main(void) {
    //SetConfigFlags(FLAG_WINDOW_RESIZABLE); // Allows window to be resizable by mouse dragging

    InitWindow(SCREENHEIGHT, SCREENWIDTH, "SpaceShooter");
    //InitWindow(0, 0, "SpaceShooter");
    InitGameScreen();
    SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));
    ToggleFullscreen();
    

    switch(currentScreen) {
        case LOGO: {
            InitLogoScreen();
        } break;
        case MAINMENU: {
            LoadBackground();
            InitMainMenu();
        } break;
        case GAME: {
            LoadBackground(); // Don't load background until after the intro
            InitGame();
        }
    }

    while (!WindowShouldClose()) {
        UpdateDrawingFrame();
    }

    UnloadGame();
    UnloadGameScreen();

    CloseWindow();

    return 0;
}

//Controls drawing each frame
void UpdateDrawingFrame(void) {
    switch(currentScreen) {
        case LOGO: {
            UpdateLogoScreen();
            if (FinishLogoScreen()) {
                UnloadLogoScreen();
                LoadBackground();
                currentScreen = MAINMENU;
                InitMainMenu();
            }
        } break;
        case MAINMENU: {
            UpdateMainMenu();
            if (FinishMainMenu())
            {
                UnloadMainMenu();
                currentScreen = GAME;
                InitGame();
            }
        } break;
        case GAME: {    
            UpdateGame();
        }
    }

     BeginDrawing();
        ClearBackground(RAYWHITE);
        if (IsWindowFullscreen()) {
            DrawTextureNPatch(background, backgroundinfo, (Rectangle) {0.0f, 0.0f, (float) GetScreenWidth(), (float) GetMonitorHeight(display)}, Vector2Zero(), 0.0f, RAYWHITE); // Draw's background
        } else {
            DrawTextureNPatch(background, backgroundinfo, (Rectangle) {0.0f, 0.0f, (float) GetScreenWidth(), (float) GetScreenHeight()}, Vector2Zero(), 0.0f, RAYWHITE); // Draw's background
        }
        
        switch(currentScreen) {
            case LOGO: DrawLogoScreen(); break;
            case MAINMENU: DrawMainMenu(); break;
            case GAME: DrawGame(); break;
            default: break;
        }

    EndDrawing();
}

// Loads all necessary textures for the game
void LoadBackground(void) {
    float backgroundnum = RandomNum(0, 1);
    if (backgroundnum == 0) {
        background = LoadTexture("assets/River/background.png");        
    } else {
        background = LoadTexture("assets/Desert/desert-background.png");
    }
    backgroundinfo = {(Rectangle) {0.0f, 0.0f, (float) background.width, (float) background.height}, 0, 0, 0, 0, NPATCH_NINE_PATCH};
}

// Initializes game variables/settings
void InitGameScreen(void) {
    currentScreen = MAINMENU;
    display = GetCurrentMonitor();
    fps = GetMonitorRefreshRate(display);
    SetTargetFPS(fps);
}

// Unloads all load textures, sounds, models, etc.
void UnloadGameScreen(void) {
UnloadTexture(background);
}

float RandomNum(int min, int max) {
    srand(time(0));
    return min + (rand() % (max + 1));
}

//----------------------------------------------------------------------------------
// Object Functions
//----------------------------------------------------------------------------------
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

Object makeEnemy(Texture2D enemy, float scale, float x, float y) {
    Object newEnemy;
    newEnemy.drawRec = {0.0f, 0.0f, (float)((enemy.width/2.0f)), (float)enemy.height};
    newEnemy.position = {x, y, (float)((enemy.width/5.0f) * scale), (float)((enemy.height/2.0f) * scale)};
    newEnemy.origin = {(float)(newEnemy.position.width/2.0f), (float)(newEnemy.position.height/2.0f)};
    newEnemy.rotation = 0.0f;
    newEnemy.texture = enemy;
    return newEnemy;
}

Object makeLogo(Texture2D logopng, float scale) {
    Object newLogo;
    newLogo.drawRec = {0.0f, 0.0f, (float)(logopng.width), (float)(logopng.height)};
    newLogo.position = {(float)(GetScreenWidth()/2.0f), (float)(GetScreenHeight()/6.0f), (float)((logopng.width * 0.8) * scale), (float)((logopng.height * 0.7) * scale)};
    newLogo.origin = {(float)(newLogo.position.width/2.0f), (float)(newLogo.position.height/2.0f)};
    newLogo.rotation = 0.0f;
    return newLogo;
}

//TODO: Center play button 
Object makeMainMenuButton(Texture2D buttonpng, float x, float y, float scale) {
    Object newButton;
    float width = (buttonpng.width * 1.5f) * scale;
    newButton.drawRec = {0.0f, 0.0f, (float)(buttonpng.width), (float)(buttonpng.height/3.0f)};
    newButton.position = {(float)(x - ((width/2.0f) - (width/16.0f))), y, (float)(width), (float)((buttonpng.height * 0.5f) * scale)}; // The width is divided by 16 to subtract the # of pixels between the actual button and edge of image after the width is scaled
    newButton.origin = {0.0f, 0.0f};
    newButton.rotation = 0.0f;
    newButton.texture = buttonpng;
    return newButton;
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
