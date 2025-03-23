#include "raylib.h"

#define GRAVITY 600.0f        // Gravedad (pixels/s^2)
#define JUMP_FORCE 350.0f     // Fuerza del salto
#define DASH_SPEED 800.0f     // Velocidad del dash
#define DASH_TIME 0.2f        // Duración del dash (segundos)

typedef struct Player {
    Vector2 position;
    Vector2 velocity;
    float speed;
    int dashCharges;
    int jumpCount;
    bool isDashing;
    float dashTimer;
    Vector2 dashDirection;
} Player;

int main(void) {
    // Configuración de la ventana
    const int screenWidth = 800;
    const int screenHeight = 450;
    const float groundLevel = screenHeight - 20;
    InitWindow(screenWidth, screenHeight, "Neon Rush - Dash Estilo Celeste");

    // Inicialización del jugador
    Player player = { { screenWidth/2.0f, groundLevel }, {0, 0}, 200.0f, 1, 0, false, 0, {0, 0} };

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        // Si el dash está activo, ignoramos la gravedad y usamos solo la velocidad del dash
        if (player.isDashing) {
            player.position.x += player.dashDirection.x * DASH_SPEED * deltaTime;
            player.position.y += player.dashDirection.y * DASH_SPEED * deltaTime;
            player.dashTimer -= deltaTime;

            if (player.dashTimer <= 0) {
                player.isDashing = false;
                player.velocity.y = 0;  // 🔥🔥 SOLUCIÓN: Evita el "salto fantasma" al terminar el dash 🔥🔥
            }
        } else {
            // Movimiento con A y D
            if (IsKeyDown(KEY_D)) player.velocity.x = player.speed;
            else if (IsKeyDown(KEY_A)) player.velocity.x = -player.speed;
            else player.velocity.x = 0;

            // Salto y doble salto
            if (IsKeyPressed(KEY_SPACE) && player.jumpCount < 2) {
                player.velocity.y = -JUMP_FORCE;
                player.jumpCount++;
            }

            // Aplicación de la gravedad
            player.velocity.y += GRAVITY * deltaTime;
            player.position.x += player.velocity.x * deltaTime;
            player.position.y += player.velocity.y * deltaTime;
        }

        // Dash - Se activa con LEFT CONTROL
        if (IsKeyPressed(KEY_LEFT_CONTROL) && player.dashCharges > 0) {
            player.isDashing = true;
            player.dashTimer = DASH_TIME;
            player.dashCharges--;

            // Determinar dirección del dash según teclas presionadas
            player.dashDirection = (Vector2){0, 0};
            if (IsKeyDown(KEY_D)) player.dashDirection.x = 1;
            if (IsKeyDown(KEY_A)) player.dashDirection.x = -1;
            if (IsKeyDown(KEY_W)) player.dashDirection.y = -1;
            if (IsKeyDown(KEY_S)) player.dashDirection.y = 1;

            // Si no se presiona dirección, el dash es hacia donde miraba el jugador
            if (player.dashDirection.x == 0 && player.dashDirection.y == 0) {
                player.dashDirection.x = (player.velocity.x >= 0) ? 1 : -1;
            }

            // 🔥🔥 SOLUCIÓN: Al hacer dash, cancelamos la velocidad vertical para evitar arrastrar la anterior 🔥🔥
            player.velocity.y = 0;
        }

        // Colisión con el suelo y reinicio de saltos y dash
        if (player.position.y >= groundLevel) {
            player.position.y = groundLevel;
            player.velocity.y = 0;
            player.jumpCount = 0;       // Reinicia el contador de saltos
            player.dashCharges = 1;     // Recarga UNA carga de dash
        }

        // Renderizado
        BeginDrawing();
            ClearBackground(BLACK);
            DrawCircleV(player.position, 10, RAYWHITE);
            DrawText(TextFormat("Dash Charges: %i", player.dashCharges), 10, 10, 20, LIGHTGRAY);
            DrawText(TextFormat("Jump Count: %i", player.jumpCount), 10, 40, 20, LIGHTGRAY);
            DrawText(player.isDashing ? "DASHING" : "NORMAL", 10, 70, 20, RED);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
