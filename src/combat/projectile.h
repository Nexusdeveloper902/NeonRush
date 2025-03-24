#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "raylib.h"

typedef struct Projectile {
    Vector2 position;
    Vector2 velocity;
    float radius;
    int damage;
    bool active;
} Projectile;

#define MAX_PROJECTILES 10

void ProjectileInit(Projectile *p, Vector2 position, Vector2 direction, float speed, float radius, int damage);
void ProjectileUpdate(Projectile *p, float deltaTime);
void ProjectileRender(Projectile p);

#endif // PROJECTILE_H
