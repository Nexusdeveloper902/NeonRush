#include <stddef.h>
#include "raylib.h"
#include "raymath.h"
#include "entities/player.h"
#include "HUD/hud.h"
#include "levelgen/levelgen.h"
#include "config/config.h"

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Neon Rush - Plataformero Procedural (165 FPS)");

    // Generar el nivel procedural
    Level level = GenerateLevel(screenWidth, screenHeight);

    // Inicializar el jugador en el primer nodo generado (si existen nodos)
    Player player;
    if(level.nodeCount > 0) {
        PlayerInit(&player, level.nodes[0].position);
    } else {
        PlayerInit(&player, (Vector2){screenWidth / 2.0f, screenHeight / 2.0f});
    }

    // Inicializar el HUD
    HUD hud;
    HUDInit(&hud);

    SetTargetFPS(165);

    while(!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        // Actualizar el jugador (no se utilizan colisiones con paredes, ya que el nivel es de plataformas)
        PlayerUpdate(&player, NULL, 0, deltaTime, screenHeight - 20);
        HUDUpdate(&hud, deltaTime);

        // Verificar si el jugador ha alcanzado el nodo de salida:
        // Definimos la plataforma de salida basada en el nodo de salida
        Rectangle exitPlatform = {level.nodes[level.exitIndex].position.x - PLATFORM_WIDTH/2,
                                    level.nodes[level.exitIndex].position.y - PLATFORM_HEIGHT/2,
                                    PLATFORM_WIDTH, PLATFORM_HEIGHT};
        if (CheckCollisionCircleRec(player.position, PLAYER_RADIUS, exitPlatform)) {
            // Pasar al siguiente nivel: generar un nuevo nivel y colocar al jugador en el primer nodo
            level = GenerateLevel(screenWidth, screenHeight);
            PlayerInit(&player, level.nodes[0].position);
        }

        BeginDrawing();
            ClearBackground(BLACK);
            // Renderizar el nivel (plataformas y puentes)
            LevelRender(level);
            // Renderizar el jugador y el HUD
            PlayerRender(player);
            HUDRender(hud);
            DrawFPS(10, 130);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
