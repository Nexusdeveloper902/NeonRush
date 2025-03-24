#include "raylib.h"
#include "raymath.h"
#include "entities/player.h"
#include "entities/enemy.h"
#include "HUD/hud.h"
#include "combat/combat.h"
#include "combat/projectile.h"

#define NUM_WALLS 2
#define LIGHTGREEN (Color){144,238,144,255}

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;
    const float groundLevel = screenHeight - 20;
    InitWindow(screenWidth, screenHeight, "Neon Rush - Combate y Combos (165 FPS)");

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

    // Inicializar el HUD (hud.health = 100, hud.score = 0)
    HUD hud;
    HUDInit(&hud);

    // Variables para la invencibilidad del jugador
    bool playerInvincible = false;
    float invincibleTime = 0.0f; // Invencibilidad en segundos

    // Nueva variables para la detección de contacto y bloqueo
    bool contactActive = false;
    float contactTimer = 0.0f; // Tiempo de espera (1 segundo) para bloquear el daño

    // Inicializar proyectiles
    Projectile projectiles[MAX_PROJECTILES] = {0};
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        projectiles[i].active = false;
    }

    SetTargetFPS(165);

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        // Actualizar jugador, enemigo, HUD y sistema de combate
        PlayerUpdate(&player, walls, NUM_WALLS, deltaTime, groundLevel);
        EnemyUpdate(&enemy, player.position, deltaTime, groundLevel);
        CombatPlayerChargeUpdate(deltaTime);
        CombatPlayerAttack(&player, &enemy, 1, &hud.score);
        CombatPlayerUpdate(deltaTime);
        HUDUpdate(&hud, deltaTime);

        // Sistema de ataque a distancia: presionar G lanza un proyectil
        if (IsKeyPressed(KEY_G)) {
            for (int i = 0; i < MAX_PROJECTILES; i++) {
                if (!projectiles[i].active) {
                    Vector2 direction = Vector2Subtract(GetMousePosition(), player.position);
                    ProjectileInit(&projectiles[i], player.position, direction, 500.0f, 5.0f, 20);
                    break;
                }
            }
        }

        // Actualizar proyectiles y verificar colisiones con paredes y enemigo
        for (int i = 0; i < MAX_PROJECTILES; i++) {
            if (projectiles[i].active) {
                ProjectileUpdate(&projectiles[i], deltaTime);
                for (int j = 0; j < NUM_WALLS; j++) {
                    if (CheckCollisionCircleRec(projectiles[i].position, projectiles[i].radius, walls[j])) {
                        projectiles[i].active = false;
                        break;
                    }
                }
                if (!enemy.isDead && CheckCollisionCircles(projectiles[i].position, projectiles[i].radius,
                                                           enemy.position, enemy.radius)) {
                    enemy.health -= projectiles[i].damage;
                    hud.score += projectiles[i].damage;
                    projectiles[i].active = false;
                    if (enemy.health <= 0) {
                        enemy.isDead = true;
                    }
                }
            }
        }

        // Actualizar invencibilidad del jugador
        if (playerInvincible) {
            invincibleTime -= deltaTime;
            if (invincibleTime <= 0.0f) {
                playerInvincible = false;
            }
        }

        // Detección de contacto entre jugador y enemigo para daño
        // Solo se inicia si el enemigo está en contacto y el jugador NO está atacando (tecla F)
        if (!enemy.isDead && CheckCollisionCircles(player.position, PLAYER_RADIUS, enemy.position, enemy.radius)) {
            if (!contactActive) {
                contactActive = true;
                contactTimer = 1.0f; // Un segundo de "ventana" para bloquear el daño con F
            }
        } else {
            // Reiniciar si ya no hay contacto
            contactActive = false;
            contactTimer = 0.0f;
        }

        if (contactActive) {
            contactTimer -= deltaTime;
            // Si el jugador presiona F durante este tiempo, se bloquea el daño (se paraliza el enemigo)
            if (IsKeyPressed(KEY_F)) {
                // "Paralizamos" al enemigo durante 1 segundo (podríamos, por ejemplo, poner su velocidad a 0)
                enemy.velocity.x = 0;
                enemy.vy = 0;
                contactActive = false;
                contactTimer = 0.0f;
                // Aquí se podría agregar un feedback visual adicional para indicar el bloqueo
            } else if (contactTimer <= 0.0f) {
                // Si se agota el tiempo sin presionar F, se aplica el daño
                hud.health -= 20;  // Daño instantáneo reducido
                playerInvincible = true;
                invincibleTime = 0.5f; // Invencibilidad por 0.5 segundos
                contactActive = false;
                contactTimer = 0.0f;
                if (hud.health < 0) hud.health = 0;
            }
        }

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
            for (int i = 0; i < MAX_PROJECTILES; i++) {
                if (projectiles[i].active) {
                    ProjectileRender(projectiles[i]);
                }
            }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
