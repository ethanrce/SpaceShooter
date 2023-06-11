#ifndef SCREENS_H
#define SCREENS_H
#include <iostream>
using std::string;

enum GameScreen {LOGO, MAINMENU, GAME};
enum GamePhase {TRANSITION, INWAVE};
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
void UpdateMainMenu(void);
void DrawMainMenu(void);
void UnloadMainMenu();
bool FinishMainMenu(void);
string CheckButtonHover(void);

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