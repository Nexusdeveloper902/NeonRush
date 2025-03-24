#include "player.h"
#include "raymath.h"

void PlayerInit(Player *player, Vector2 startPosition) {
    player->position = startPosition;
    player->velocity = (Vector2){0, 0};
    player->speed = 200.0f;
    player->dashCharges = 1;
    player->jumpCount = 0;
    player->isDashing = false;
    player->dashTimer = 0;
    player->dashDirection = (Vector2){0, 0};
    player->isGrappling = false;
    player->grappleAnchor = (Vector2){0, 0};
}

void PlayerUpdate(Player *player, Rectangle *walls, int numWalls, float deltaTime, float groundLevel) {
    // Activación del Grappling Hook con la tecla E
    if (IsKeyPressed(KEY_E) && !player->isDashing) {
        player->isGrappling = true;
        player->grappleAnchor = GetMousePosition();
    }

    if (player->isGrappling) {
        Vector2 toAnchor = Vector2Subtract(player->grappleAnchor, player->position);
        float distance = Vector2Length(toAnchor);
        if (distance < GRAPPLE_THRESHOLD) {
            player->isGrappling = false;
        } else {
            Vector2 dir = Vector2Normalize(toAnchor);
            player->velocity = Vector2Add(player->velocity, Vector2Scale(dir, GRAPPLE_ACCELERATION * deltaTime));
            float currentSpeed = Vector2Length(player->velocity);
            if (currentSpeed > MAX_GRAPPLE_SPEED) {
                player->velocity = Vector2Scale(Vector2Normalize(player->velocity), MAX_GRAPPLE_SPEED);
            }
            Vector2 newPos = Vector2Add(player->position, Vector2Scale(player->velocity, deltaTime));
            bool collision = false;
            for (int i = 0; i < numWalls; i++) {
                if (CheckCollisionCircleRec(newPos, PLAYER_RADIUS, walls[i])) {
                    collision = true;
                    break;
                }
            }
            if (collision) {
                player->isGrappling = false;
            } else {
                player->position = newPos;
            }
        }
    }
    else if (player->isDashing) {
        Vector2 oldPos = player->position;
        player->position.x += player->dashDirection.x * DASH_SPEED * deltaTime;
        player->position.y += player->dashDirection.y * DASH_SPEED * deltaTime;
        for (int i = 0; i < numWalls; i++) {
            if (CheckCollisionCircleRec(player->position, PLAYER_RADIUS, walls[i])) {
                player->position = oldPos;
                player->isDashing = false;
                break;
            }
        }
        player->dashTimer -= deltaTime;
        if (player->dashTimer <= 0) {
            player->isDashing = false;
            player->velocity.y = 0;
        }
    }
    else {
        // Movimiento horizontal
        if (IsKeyDown(KEY_D))
            player->velocity.x = player->speed;
        else if (IsKeyDown(KEY_A))
            player->velocity.x = -player->speed;
        else
            player->velocity.x = 0;

        // Salto y doble salto
        if (IsKeyPressed(KEY_SPACE) && player->jumpCount < 2) {
            player->velocity.y = -JUMP_FORCE;
            player->jumpCount++;
        }

        player->velocity.y += GRAVITY * deltaTime;

        // Resolver colisiones en eje X
        float oldX = player->position.x;
        player->position.x += player->velocity.x * deltaTime;
        for (int i = 0; i < numWalls; i++) {
            if (CheckCollisionCircleRec((Vector2){player->position.x, player->position.y}, PLAYER_RADIUS, walls[i])) {
                player->position.x = oldX;
                break;
            }
        }

        // Resolver colisiones en eje Y
        bool landed = false;
        float oldY = player->position.y;
        player->position.y += player->velocity.y * deltaTime;
        for (int i = 0; i < numWalls; i++) {
            if (CheckCollisionCircleRec((Vector2){player->position.x, player->position.y}, PLAYER_RADIUS, walls[i])) {
                player->position.y = oldY;
                if (player->velocity.y > 0 && (oldY + PLAYER_RADIUS) <= walls[i].y + 5)
                    landed = true;
                player->velocity.y = 0;
                break;
            }
        }

        if (player->position.y >= groundLevel) {
            player->position.y = groundLevel;
            player->velocity.y = 0;
            landed = true;
        }

        if (landed) {
            player->jumpCount = 0;
            player->dashCharges = 1;
            player->isDashing = false;
        }

        // Activar dash con LEFT CONTROL
        if (IsKeyPressed(KEY_LEFT_CONTROL) && player->dashCharges > 0) {
            player->isDashing = true;
            player->dashTimer = DASH_TIME;
            player->dashCharges--;
            player->dashDirection = (Vector2){0, 0};
            if (IsKeyDown(KEY_D)) player->dashDirection.x = 1;
            if (IsKeyDown(KEY_A)) player->dashDirection.x = -1;
            if (IsKeyDown(KEY_W)) player->dashDirection.y = -1;
            if (IsKeyDown(KEY_S)) player->dashDirection.y = 1;
            if (player->dashDirection.x == 0 && player->dashDirection.y == 0)
                player->dashDirection.x = (player->velocity.x >= 0) ? 1 : -1;
            player->velocity.y = 0;
        }
    }

    if (!player->isDashing) {
        player->velocity = Vector2Scale(player->velocity, 1.0f - AIR_FRICTION * deltaTime);
    }
}

void PlayerRender(Player player) {
    DrawCircleV(player.position, PLAYER_RADIUS, RAYWHITE);
}
