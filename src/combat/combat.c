#include "combat.h"
#include "raymath.h"

static float attackCooldown = 0.0f;
static float attackCharge = 0.0f;
static bool chargingAttack = false;

static float attackFeedbackTime = 0.0f;
static Vector2 feedbackPosition = { 0, 0 };

void CombatPlayerAttack(Player *player, Enemy *enemies, int numEnemies, int *score) {
    if (!IsKeyDown(KEY_F) && chargingAttack) {
        // El daño base se incrementa según la carga
        int damage = (int)(10 * (1 + attackCharge));
        for (int i = 0; i < numEnemies; i++) {
            if (!enemies[i].isDead &&
                CheckCollisionCircles(player->position, PLAYER_ATTACK_RADIUS + attackCharge * 5,
                                      enemies[i].position, enemies[i].radius)) {
                enemies[i].health -= damage;
                *score += damage;
                // Feedback visual
                feedbackPosition = enemies[i].position;
                attackFeedbackTime = 0.3f;
                // Si la salud es 0 o menor, marcar al enemigo como muerto
                if (enemies[i].health <= 0) {
                    enemies[i].isDead = true;
                }
            }
        }
        attackCooldown = ATTACK_COOLDOWN;
        attackCharge = 0.0f;
        chargingAttack = false;
    }
}

void CombatPlayerUpdate(float deltaTime) {
    if (attackCooldown > 0.0f) {
        attackCooldown -= deltaTime;
        if (attackCooldown < 0.0f)
            attackCooldown = 0.0f;
    }
    if (attackFeedbackTime > 0.0f) {
        attackFeedbackTime -= deltaTime;
        if (attackFeedbackTime < 0.0f)
            attackFeedbackTime = 0.0f;
    }
}

void CombatPlayerChargeUpdate(float deltaTime) {
    if (IsKeyDown(KEY_F) && attackCooldown <= 0.0f) {
        chargingAttack = true;
        attackCharge += deltaTime;
        if (attackCharge > MAX_ATTACK_CHARGE)
            attackCharge = MAX_ATTACK_CHARGE;
    }
}

float GetPlayerAttackCharge() {
    return attackCharge;
}

void CombatRenderFeedback(void) {
    if (attackFeedbackTime > 0.0f) {
        float alphaFactor = attackFeedbackTime / 0.3f;
        Color feedbackColor = (Color){255, 255, 0, (unsigned char)(alphaFactor * 255)};
        float radius = 20.0f * (1.0f + (1.0f - alphaFactor));
        DrawCircleV(feedbackPosition, radius, feedbackColor);
    }
}
