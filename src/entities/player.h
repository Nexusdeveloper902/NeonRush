#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "../config/config.h"

typedef struct Player {
    Vector2 position;
    Vector2 velocity;
    float speed;
    int dashCharges;
    int jumpCount;
    bool isDashing;
    float dashTimer;
    Vector2 dashDirection;
    bool isGrappling;
    Vector2 grappleAnchor;
} Player;

void PlayerInit(Player *player, Vector2 startPosition);
void PlayerUpdate(Player *player, Rectangle *walls, int numWalls, float deltaTime, float groundLevel);
void PlayerRender(Player player);

#endif // PLAYER_H
