#ifndef SCREENS_H
#define SCREENS_H

enum GameScreen {LOGO, MAINMENU, GAME};

//----------------------------------------------------------------------------------
// Main Functions Declaration
//----------------------------------------------------------------------------------
void UpdateDrawingFrame(void);
void LoadTextures(void);
void InitGame(void);
void MakeLaser(void);
void UnloadGame(void);
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
void InitMainMenu(Texture2D background);
void UpdateMainMenu(void);
void DrawMainMenu(void);
void UnloadMainMenu(void);
int FinishMainMenu(void);


#endif