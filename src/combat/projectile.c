#include "projectile.h"
#include "raymath.h"

void ProjectileInit(Projectile *p, Vector2 position, Vector2 direction, float speed, float radius, int damage) {
    p->position = position;
    p->velocity = Vector2Scale(Vector2Normalize(direction), speed);
    p->radius = radius;
    p->damage = damage;
    p->active = true;
}

void ProjectileUpdate(Projectile *p, float deltaTime) {
    if (!p->active) return;
    p->position = Vector2Add(p->position, Vector2Scale(p->velocity, deltaTime));
    // Desactivar si sale de la pantalla (ajusta según tus dimensiones)
    if (p->position.x < 0 || p->position.x > 800 || p->position.y < 0 || p->position.y > 450)
        p->active = false;
}

void ProjectileRender(Projectile p) {
    if (!p.active) return;
    DrawCircleV(p.position, p.radius, YELLOW);
}
