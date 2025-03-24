#ifndef COMBAT_H
#define COMBAT_H

#include "raylib.h"
#include "../entities/player.h"
#include "../entities/enemy.h"

#define PLAYER_ATTACK_RADIUS 20.0f
#define ATTACK_COOLDOWN 0.5f
#define MAX_ATTACK_CHARGE 2.0f    // Tiempo máximo para cargar el ataque (segundos)

void CombatPlayerAttack(Player *player, Enemy *enemies, int numEnemies, int *score);
void CombatPlayerUpdate(float deltaTime);
void CombatPlayerChargeUpdate(float deltaTime);
float GetPlayerAttackCharge();
void CombatRenderFeedback(void);

#endif // COMBAT_H
