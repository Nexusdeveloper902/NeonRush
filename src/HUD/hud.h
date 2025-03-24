#ifndef HUD_H
#define HUD_H

#include "raylib.h"

typedef struct HUD {
    int health;
    int score;
} HUD;

void HUDInit(HUD *hud);
void HUDUpdate(HUD *hud, float deltaTime);
void HUDRender(HUD hud);

#endif // HUD_H
