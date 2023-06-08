#include <raylib.h>
#include <raymath.h>
#include <iostream>
using std::cout;
#include "objects.h"
#include "screens.h"
#include <cstdlib>

#define SCREENHEIGHT 1200
#define SCREENWIDTH 1200
//#define STORAGE_DATA_FILE "storage.data"

GameScreen currentScreen;
Texture2D background;
NPatchInfo backgroundinfo;
int display2;
int fps2;

int main(void) {
    //SetConfigFlags(FLAG_WINDOW_RESIZABLE); // Allows window to be resizable by mouse dragging

    InitWindow(SCREENHEIGHT, SCREENWIDTH, "SpaceShooter");
    //InitWindow(0, 0, "SpaceShooter");
    InitGameScreen();
    //SetWindowSize(GetMonitorWidth(display2), GetMonitorHeight(display2));
    //ToggleFullscreen();

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
                InitMainMenu();
                currentScreen = MAINMENU;
            }
        } break;
        case MAINMENU: {
            UpdateMainMenu();
            if (FinishMainMenu()) {
                UnloadMainMenu();
                InitGame();
                currentScreen = GAME;
            }
        } break;
        case GAME: {    
            UpdateGame();
            if (FinishGame()) {
                UnloadGame();
                InitMainMenu();
                currentScreen = MAINMENU;
            }
        }
    }

     BeginDrawing();
        ClearBackground(RAYWHITE);
        if (IsWindowFullscreen()) {
            DrawTextureNPatch(background, backgroundinfo, (Rectangle) {0.0f, 0.0f, (float) GetScreenWidth(), (float) GetMonitorHeight(display2)}, Vector2Zero(), 0.0f, RAYWHITE); // Draw's background
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
    display2 = GetCurrentMonitor();
    fps2 = GetMonitorRefreshRate(display2);
    SetTargetFPS(fps2);
    srand(time(NULL));
}

// Unloads all load textures, sounds, models, etc.
void UnloadGameScreen(void) {
UnloadTexture(background);
}

float RandomNum(int min, int max) {
    return min + (rand() % (max + 1));
}

//------------------------------------------------------------------------  ----------
// Object Functions
//----------------------------------------------------------------------------------
Object makePlayer(Texture2D ship, float scale, int ScreenWidth, int ScreenHeight) {
    Object player;
    player.drawRec = {0.0f, 0.0f, (float)((ship.width/5.0f)), (float)(ship.height/2.0f)};
    player.position = {(float)(GetScreenWidth()/2.0f), (float)(GetScreenHeight()/2.0f - 100), (float)(ship.width/5.0f) * scale, (float)(ship.height/2.0f) * scale};
    player.origin = {(float)(player.position.width/2.0f), (float)(player.position.height/2.0f)}; 
    player.rotation = 0.0f;
    player.name = "player";
    return player;
}

Object shootLaser(float x, float y, float rot, float textwidth, float textheight, float scale, const char *obj) {
    Object newLaser;
    if (std::string(obj) == "player") {
        newLaser.drawRec = {0.0f, textheight/2.0f, textwidth/2.0f, textheight/2.0f};
        newLaser.rotation = rot; 
        newLaser.name = obj;
    } else {
        newLaser.drawRec = {0.0f, 0.0f, textwidth/2.0f, textheight/2.0f}; //textheight/2.0f
        newLaser.rotation = (rot + 180.0f);
        newLaser.name = "enemy";
    }
    newLaser.position = {x, y, textwidth/2.0f * scale, textheight/2.0f * scale};
    newLaser.origin = {textwidth/2.0f, textheight/2.0f};
    
    return newLaser;
}

Object makeEnemy(Texture2D enemy, float scale, float x, float y, const char *obj) {
    Object newEnemy;
    newEnemy.drawRec = {0.0f, 0.0f, (float)((enemy.width/2.0f)), (float)enemy.height};
    newEnemy.position = {x, y, (float)((enemy.width/5.0f) * scale), (float)((enemy.height/2.0f) * scale)};
    newEnemy.origin = {(float)(newEnemy.position.width/2.0f), (float)(newEnemy.position.height/2.0f)};
    newEnemy.rotation = 0.0f;
    newEnemy.texture = enemy;
    newEnemy.frame = 0;
    newEnemy.name = obj;
    newEnemy.wpindex = 0;
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
    newButton.origin =  {0.0f, 0.0f};
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
    newExplosion.frame = 0;
    return newExplosion;
}

Waypoint makeWaypoint(int x, int y) {
    Waypoint newwp;
    newwp.x = x;
    newwp.y = y;
    return newwp;
}

// Save integer value to storage file (to defined position)
// NOTE: Storage positions is directly related to file memory layout (4 bytes each integer)
/*
bool SaveStorageValue(unsigned int position, int value)
{
    bool success = false;
    unsigned int dataSize = 0;
    unsigned int newDataSize = 0;
    unsigned char *fileData = LoadFileData(STORAGE_DATA_FILE, &dataSize);
    unsigned char *newFileData = NULL;

    if (fileData != NULL)
    {
        if (dataSize <= (position*sizeof(int)))
        {
            // Increase data size up to position and store value
            newDataSize = (position + 1)*sizeof(int);
            newFileData = (unsigned char *)RL_REALLOC(fileData, newDataSize);

            if (newFileData != NULL)
            {
                // RL_REALLOC succeded
                int *dataPtr = (int *)newFileData;
                dataPtr[position] = value;
            }
            else
            {
                // RL_REALLOC failed
                TraceLog(LOG_WARNING, "FILEIO: [%s] Failed to realloc data (%u), position in bytes (%u) bigger than actual file size", STORAGE_DATA_FILE, dataSize, position*sizeof(int));

                // We store the old size of the file
                newFileData = fileData;
                newDataSize = dataSize;
            }
        }
        else
        {
            // Store the old size of the file
            newFileData = fileData;
            newDataSize = dataSize;

            // Replace value on selected position
            int *dataPtr = (int *)newFileData;
            dataPtr[position] = value;
        }

        success = SaveFileData(STORAGE_DATA_FILE, newFileData, newDataSize);
        RL_FREE(newFileData);

        TraceLog(LOG_INFO, "FILEIO: [%s] Saved storage value: %i", STORAGE_DATA_FILE, value);
    }
    else
    {
        TraceLog(LOG_INFO, "FILEIO: [%s] File created successfully", STORAGE_DATA_FILE);

        dataSize = (position + 1)*sizeof(int);
        fileData = (unsigned char *)RL_MALLOC(dataSize);
        int *dataPtr = (int *)fileData;
        dataPtr[position] = value;

        success = SaveFileData(STORAGE_DATA_FILE, fileData, dataSize);
        UnloadFileData(fileData);

        TraceLog(LOG_INFO, "FILEIO: [%s] Saved storage value: %i", STORAGE_DATA_FILE, value);
    }

    return success;
}

// Load integer value from storage file (from defined position)
// NOTE: If requested position could not be found, value 0 is returned
int LoadStorageValue(unsigned int position)
{
    int value = 0;
    unsigned int dataSize = 0;
    unsigned char *fileData = LoadFileData(STORAGE_DATA_FILE, &dataSize);

    if (fileData != NULL)
    {
        if (dataSize < (position*4)) TraceLog(LOG_WARNING, "FILEIO: [%s] Failed to find storage position: %i", STORAGE_DATA_FILE, position);
        else
        {
            int *dataPtr = (int *)fileData;
            value = dataPtr[position];
        }

        UnloadFileData(fileData);

        TraceLog(LOG_INFO, "FILEIO: [%s] Loaded storage value: %i", STORAGE_DATA_FILE, value);
    }

    return value;
}
*/