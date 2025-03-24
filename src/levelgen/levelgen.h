#ifndef LEVELGEN_H
#define LEVELGEN_H

#include "raylib.h"

#define MAX_NODES 50
#define MAX_CONNECTIONS 200

typedef struct {
    Vector2 position;
} Node;

typedef struct {
    int a, b;
} Connection;

typedef struct {
    Node nodes[MAX_NODES];
    int nodeCount;
    Connection connections[MAX_CONNECTIONS];
    int connectionCount;
    int exitIndex;  // Índice del nodo de salida
} Level;

Level GenerateLevel(int screenWidth, int screenHeight);
void LevelRender(Level level);

#endif // LEVELGEN_H
