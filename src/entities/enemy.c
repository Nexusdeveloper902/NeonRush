#include "enemy.h"
#include "raymath.h"
#include <math.h>

#define GRAVITY 600.0f

void EnemyInit(Enemy *enemy, Vector2 position, float leftBound, float rightBound, float radius) {
    enemy->position = position;
    enemy->leftBound = leftBound;
    enemy->rightBound = rightBound;
    enemy->radius = radius;
    enemy->velocity = (Vector2){ENEMY_PATROL_SPEED, 0};
    enemy->vy = 0;
    enemy->state = PATROL;
    enemy->health = 100;     // Salud inicial
    enemy->isDead = false;   // No está muerto
}

void EnemyUpdate(Enemy *enemy, Vector2 playerPosition, float deltaTime, float groundLevel) {
    if (enemy->isDead) return;  // Si el enemigo está muerto, no se actualiza

    float distToPlayer = Vector2Distance(enemy->position, playerPosition);
    enemy->state = (distToPlayer < ENEMY_DETECTION_RANGE) ? CHASE : PATROL;

    if (enemy->state == CHASE) {
        Vector2 direction = Vector2Normalize(Vector2Subtract(playerPosition, enemy->position));
        enemy->velocity.x = direction.x * ENEMY_CHASE_SPEED;
        if (playerPosition.y < enemy->position.y &&
            fabs(playerPosition.x - enemy->position.x) < ENEMY_JUMP_HORIZONTAL_THRESHOLD &&
            enemy->position.y >= groundLevel - enemy->radius - 1) {
            enemy->vy = -ENEMY_JUMP_FORCE;
            }
    } else {
        if (enemy->position.x < enemy->leftBound || enemy->position.x > enemy->rightBound)
            enemy->velocity.x *= -1;
    }

    enemy->position.x += enemy->velocity.x * deltaTime;

    enemy->vy += GRAVITY * deltaTime;
    enemy->position.y += enemy->vy * deltaTime;
    if (enemy->position.y >= groundLevel - enemy->radius) {
        enemy->position.y = groundLevel - enemy->radius;
        enemy->vy = 0;
    }
}

void EnemyRender(Enemy enemy) {
    if (enemy.isDead) return;  // No se renderiza si está muerto
    DrawCircleV(enemy.position, enemy.radius, RED);
}
