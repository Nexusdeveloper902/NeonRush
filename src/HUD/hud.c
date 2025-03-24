#include "hud.h"
#include "raylib.h"

void HUDInit(HUD *hud) {
    hud->health = 100;
    hud->score = 0;
}

void HUDUpdate(HUD *hud, float deltaTime) {
    // Actualizaciones del HUD (si es necesario)
}

void HUDRender(HUD hud) {
    DrawText(TextFormat("Health: %i", hud.health), 10, 10, 20, LIGHTGRAY);
    DrawText(TextFormat("Score: %i", hud.score), 10, 40, 20, LIGHTGRAY);
}
