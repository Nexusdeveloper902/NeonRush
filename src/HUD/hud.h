#ifndef HUD_H
#define HUD_H

#include "raylib.h"

// Estructura para almacenar la información del HUD
typedef struct HUD {
    int health;  // Salud del jugador
    int lives;   // Vidas restantes
    int score;   // Puntuación acumulada
} HUD;

// Inicializa el HUD con valores iniciales
void HUDInit(HUD *hud);

// Actualiza el HUD (por ejemplo, la puntuación, u otros efectos)
void HUDUpdate(HUD *hud, float deltaTime);

// Renderiza la información del HUD en pantalla
void HUDRender(HUD hud);

#endif // HUD_H
