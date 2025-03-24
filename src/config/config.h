#ifndef CONFIG_H
#define CONFIG_H

// ---------------------------------
// Configuración del Jugador
// ---------------------------------
#define PLAYER_RADIUS 10
#define PLAYER_SPEED 200.0f
#define GRAVITY 600.0f
#define JUMP_FORCE 350.0f
#define DASH_SPEED 800.0f
#define DASH_TIME 0.2f
#define GRAPPLE_THRESHOLD 10
#define GRAPPLE_ACCELERATION 1500.0f
#define MAX_GRAPPLE_SPEED 1000.0f
#define AIR_FRICTION 1.5f

// ---------------------------------
// Configuración del Enemigo
// ---------------------------------
#define ENEMY_PATROL_SPEED 100.0f
#define ENEMY_CHASE_SPEED 150.0f
#define ENEMY_DETECTION_RANGE 200.0f
#define ENEMY_JUMP_FORCE 300.0f
#define ENEMY_JUMP_HORIZONTAL_THRESHOLD 100.0f
#define ENEMY_INITIAL_HEALTH 100

// ---------------------------------
// Configuración de Plataformas (Nivel)
// ---------------------------------
#define PLATFORM_WIDTH 60
#define PLATFORM_HEIGHT 10

// ---------------------------------
// Configuración de Estados del Juego
// ---------------------------------
typedef enum {
    STATE_MENU,
    STATE_GAMEPLAY,
    STATE_PAUSE,
    STATE_GAMEOVER
} GameState;

#endif // CONFIG_H
