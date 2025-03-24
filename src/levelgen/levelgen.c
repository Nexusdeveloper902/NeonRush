#include "levelgen.h"
#include "raymath.h"
#include <stdlib.h>

#define PLATFORM_WIDTH 60
#define PLATFORM_HEIGHT 10

Level GenerateLevel(int screenWidth, int screenHeight) {
    Level level = {0};
    // Generar un número aleatorio de nodos entre 10 y MAX_NODES
    int numNodes = 10 + rand() % (MAX_NODES - 10 + 1);
    level.nodeCount = numNodes;
    for (int i = 0; i < numNodes; i++) {
        level.nodes[i].position.x = (float)(rand() % screenWidth);
        level.nodes[i].position.y = (float)(rand() % screenHeight);
    }

    level.connectionCount = 0;
    // Conectar cada nodo con su vecino más cercano
    for (int i = 0; i < numNodes; i++) {
        float minDist = 1e10f;
        int nearest = -1;
        for (int j = 0; j < numNodes; j++) {
            if (i == j) continue;
            float dist = Vector2Distance(level.nodes[i].position, level.nodes[j].position);
            if (dist < minDist) {
                minDist = dist;
                nearest = j;
            }
        }
        if (nearest >= 0 && level.connectionCount < MAX_CONNECTIONS) {
            level.connections[level.connectionCount].a = i;
            level.connections[level.connectionCount].b = nearest;
            level.connectionCount++;
        }
    }
    // Definir el nodo de salida como el último nodo
    level.exitIndex = numNodes - 1;
    return level;
}

void LevelRender(Level level) {
    // Dibujar puentes: Para cada conexión se dibuja un puente como un rectángulo rotado
    for (int i = 0; i < level.connectionCount; i++) {
        int a = level.connections[i].a;
        int b = level.connections[i].b;
        Vector2 p1 = level.nodes[a].position;
        Vector2 p2 = level.nodes[b].position;
        Vector2 center = {(p1.x + p2.x) / 2, (p1.y + p2.y) / 2};
        float length = Vector2Distance(p1, p2);
        float angle = atan2f(p2.y - p1.y, p2.x - p1.x);
        Rectangle bridge = {center.x - length/2, center.y - PLATFORM_HEIGHT/2, length, PLATFORM_HEIGHT};
        DrawRectanglePro(bridge, (Vector2){length/2, PLATFORM_HEIGHT/2}, angle * RAD2DEG, DARKGRAY);
    }

    // Dibujar plataformas (nodos) como rectángulos
    for (int i = 0; i < level.nodeCount; i++) {
        Rectangle platform = {level.nodes[i].position.x - PLATFORM_WIDTH/2, level.nodes[i].position.y - PLATFORM_HEIGHT/2, PLATFORM_WIDTH, PLATFORM_HEIGHT};
        // El nodo de salida se dibuja en GOLD, los demás en DARKGRAY
        Color platColor = (i == level.exitIndex) ? GOLD : DARKGRAY;
        DrawRectangleRec(platform, platColor);
    }
}
