#include "combat.h"
#include "raymath.h"
#include <stdio.h>

// Variables estáticas para el ataque cargado y cooldown
static float attackCooldown = 0.0f;
static float attackCharge = 0.0f;
static bool chargingAttack = false;

// Variables para el feedback visual del ataque
static float attackFeedbackTime = 0.0f;
static Vector2 feedbackPosition = { 0, 0 };

// Variables para el sistema de combos
static int comboCount = 0;
static float comboTimer = 0.0f;
static const float COMBO_WINDOW = 1.0f; // Tiempo en segundos para encadenar ataques

// Multiplicador extra para combos aéreos
static const float AERIAL_COMBO_MULTIPLIER = 1.2f;

void CombatPlayerAttack(Player *player, Enemy *enemies, int numEnemies, int *score) {
    // Se ejecuta el ataque cuando se suelta la tecla F y se estaba cargando el ataque
    if (!IsKeyDown(KEY_F) && chargingAttack) {
        // Actualizar el combo: si aún hay tiempo en la ventana, incrementa; de lo contrario, reinicia
        if (comboTimer > 0.0f) {
            comboCount++;
        } else {
            comboCount = 1;
        }
        comboTimer = COMBO_WINDOW;

        int baseDamage = (int)(10 * (1 + attackCharge));
        // Multiplicador base del combo: 1 + 0.5*(comboCount - 1)
        float multiplier = 1.0f + 0.5f * (comboCount - 1);
        // Si el jugador está en el aire (suponemos que si jumpCount > 0, aún no ha aterrizado) se añade un extra
        if (player->jumpCount > 0) {
            multiplier *= AERIAL_COMBO_MULTIPLIER;
        }
        int damage = (int)(baseDamage * multiplier);

        for (int i = 0; i < numEnemies; i++) {
            if (!enemies[i].isDead &&
                CheckCollisionCircles(player->position, PLAYER_ATTACK_RADIUS + attackCharge * 5,
                                      enemies[i].position, enemies[i].radius)) {
                enemies[i].health -= damage;
                *score += damage;
                feedbackPosition = enemies[i].position;
                attackFeedbackTime = 0.3f;
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
    // Actualizar el temporizador de combo
    if (comboTimer > 0.0f) {
        comboTimer -= deltaTime;
        if (comboTimer <= 0.0f) {
            comboTimer = 0.0f;
            comboCount = 0; // Reiniciar combo si se excede la ventana
        }
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

float GetPlayerAttackCharge(void) {
    return attackCharge;
}

int GetComboCount(void) {
    return comboCount;
}

void CombatRenderFeedback(void) {
    if (attackFeedbackTime > 0.0f) {
        float alphaFactor = attackFeedbackTime / 0.3f;
        Color feedbackColor = (Color){255, 255, 0, (unsigned char)(alphaFactor * 255)};
        float radius = 20.0f * (1.0f + (1.0f - alphaFactor));
        DrawCircleV(feedbackPosition, radius, feedbackColor);
        if (comboCount > 1) {
            char comboText[32];
            sprintf(comboText, "Combo x%d", comboCount);
            int textWidth = MeasureText(comboText, 20);
            DrawText(comboText, feedbackPosition.x - textWidth / 2, feedbackPosition.y - (int)radius - 20, 20, feedbackColor);
        }
    }
}
