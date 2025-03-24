#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include "../config/config.h"

typedef enum { PATROL, CHASE } EnemyState;

typedef struct Enemy {
    Vector2 position;
    Vector2 velocity;
    float vy;
    float leftBound;
    float rightBound;
    float radius;
    EnemyState state;
    int health;
    bool isDead;
} Enemy;

void EnemyInit(Enemy *enemy, Vector2 position, float leftBound, float rightBound, float radius);
void EnemyUpdate(Enemy *enemy, Vector2 playerPosition, float deltaTime, float groundLevel);
void EnemyRender(Enemy enemy);

#endif // ENEMY_H
