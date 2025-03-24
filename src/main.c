#include "raylib.h"
#include "raymath.h"
#include "entities/player.h"
#include "entities/enemy.h"
#include "HUD/hud.h"
#include "combat/combat.h"

#define NUM_WALLS 2
#define LIGHTGREEN (Color){144,238,144,255}

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;
    const float groundLevel = screenHeight - 20;
    InitWindow(screenWidth, screenHeight, "Neon Rush - Sistema de Combate (165 FPS)");

    // Definir paredes
    Rectangle walls[NUM_WALLS] = {
        {300, 300, 200, 20},
        {100, 150, 50, 100}
    };

    // Inicializar el jugador
    Player player;
    PlayerInit(&player, (Vector2){screenWidth/2.0f, groundLevel});

    // Inicializar el enemigo
    Enemy enemy;
    EnemyInit(&enemy, (Vector2){100, groundLevel - PLAYER_RADIUS}, 100, 300, 10);

    // Inicializar el HUD
    HUD hud;
    HUDInit(&hud);

    SetTargetFPS(165);

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        // Actualizar jugador, enemigo y HUD
        PlayerUpdate(&player, walls, NUM_WALLS, deltaTime, groundLevel);
        EnemyUpdate(&enemy, player.position, deltaTime, groundLevel);

        // Sistema de combate
        CombatPlayerChargeUpdate(deltaTime);
        CombatPlayerAttack(&player, &enemy, 1, &hud.score);
        CombatPlayerUpdate(deltaTime);
        HUDUpdate(&hud, deltaTime);

        BeginDrawing();
            ClearBackground(BLACK);
            for (int i = 0; i < NUM_WALLS; i++) {
                DrawRectangleRec(walls[i], GRAY);
            }
            if (player.isGrappling) {
                DrawLineV(player.position, player.grappleAnchor, LIGHTGREEN);
                DrawCircleV(player.grappleAnchor, 5, LIGHTGREEN);
            }
            PlayerRender(player);
            EnemyRender(enemy);
            HUDRender(hud);
            DrawText(player.isDashing ? "DASHING" : (player.isGrappling ? "GRAPPLING" : "NORMAL"), 10, 100, 20, RED);
            DrawFPS(10, 130);
            CombatRenderFeedback();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
