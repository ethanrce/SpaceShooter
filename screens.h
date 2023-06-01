#ifndef SCREENS_H
#define SCREENS_H

enum GameScreen {LOGO, MAINMENU, GAME};

void InitLogoScreen(void);
void UpdateLogoScreen(void);
void DrawLogoScreen(void);
void UnloadLogoScreen(void);
int FinishLogoScreen(void);

#endif