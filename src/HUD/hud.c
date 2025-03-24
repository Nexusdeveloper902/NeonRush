#include "hud.h"
#include "raylib.h"

void HUDInit(HUD *hud) {
    hud->health = 100;
    hud->lives = 3;
    hud->score = 0;
}

void HUDUpdate(HUD *hud, float deltaTime) {
    // Ejemplo: aumentar la puntuación gradualmente
    hud->score += (int)(10 * deltaTime);
    // Aquí se podrían agregar más actualizaciones (por ejemplo, si el jugador pierde salud)
}

void HUDRender(HUD hud) {
    DrawText(TextFormat("Health: %i", hud.health), 10, 10, 20, LIGHTGRAY);
    DrawText(TextFormat("Lives: %i", hud.lives), 10, 40, 20, LIGHTGRAY);
    DrawText(TextFormat("Score: %i", hud.score), 10, 70, 20, LIGHTGRAY);
}
