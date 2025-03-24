#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

#define PLAYER_RADIUS 10
#define GRAVITY 600.0f
#define JUMP_FORCE 350.0f
#define DASH_SPEED 800.0f
#define DASH_TIME 0.2f
#define GRAPPLE_THRESHOLD 10
#define GRAPPLE_ACCELERATION 1500.0f
#define MAX_GRAPPLE_SPEED 1000.0f
#define AIR_FRICTION 1.5f

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
