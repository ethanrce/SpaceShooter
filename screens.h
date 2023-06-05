#ifndef SCREENS_H
#define SCREENS_H

enum GameScreen {LOGO, MAINMENU, GAME};

//----------------------------------------------------------------------------------
// Main Functions Declaration
//----------------------------------------------------------------------------------
void UpdateDrawingFrame(void);
void LoadBackground(void);
void InitGameScreen(void);
void UnloadGameScreen(void);
void ChangeToScreen(GameScreen screen);

//----------------------------------------------------------------------------------
// Logo Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitLogoScreen(void);
void UpdateLogoScreen(void);
void DrawLogoScreen(void);
void UnloadLogoScreen(void);
int FinishLogoScreen(void);

//----------------------------------------------------------------------------------
// MainMenu Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitMainMenu(void);
void UpdateMainMenu(int btnState);
void DrawMainMenu(void);
void UnloadMainMenu();
int FinishMainMenu(void);
bool CheckButtonHover(void);

//----------------------------------------------------------------------------------
// Game Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitGame(void);
void LoadTextures(void);
void UpdateGame(void);
void DrawGame(void);
void UnloadGame(void);
int FinishGame(void);

#endif