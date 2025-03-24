#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"

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

#define ENEMY_PATROL_SPEED 100.0f
#define ENEMY_CHASE_SPEED 150.0f
#define ENEMY_DETECTION_RANGE 200.0f
#define ENEMY_JUMP_FORCE 300.0f
#define ENEMY_JUMP_HORIZONTAL_THRESHOLD 100.0f

void EnemyInit(Enemy *enemy, Vector2 position, float leftBound, float rightBound, float radius);
void EnemyUpdate(Enemy *enemy, Vector2 playerPosition, float deltaTime, float groundLevel);
void EnemyRender(Enemy enemy);

#endif // ENEMY_H
