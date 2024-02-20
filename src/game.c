#include "raylib.h"
#include "raymath.h"
#include <stdio.h>

typedef struct Player
{
    Vector2 position;
    int speed;
    int health;
} Player;

typedef struct Enemy
{
    Vector2 position;
    int speed;
    bool dead;
} Enemy;

#define MAX_ENEMIES 3

Player p;
Enemy enemies[MAX_ENEMIES];

Texture2D walkAnimation;
Rectangle frameRec;
int frameCounter;
int currentFrame;

void Init() 
{
    // Initialize animation
    walkAnimation = LoadTexture("textures/walk.png");
    frameRec = (Rectangle) {0.0f, 0.0f, (float)walkAnimation.width/8, (float)walkAnimation.height};
    frameCounter = 0;
    currentFrame = 0;

    // Initialize variables
    p.position = (Vector2) { 360, 200 };
    p.speed = 5;
    p.health = 100;

    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        enemies[i].speed = 2;
        enemies[i].dead = false;
    }
    enemies[0].position = (Vector2) { 100, 100 };
    enemies[1].position = (Vector2) { 500, 40 };
    enemies[2].position = (Vector2) { 50, 300 };
}

void Update()
{
    // Get input and move player
    Vector2 axis = Vector2Zero();
    axis.y = IsKeyDown(KEY_S) - IsKeyDown(KEY_W);
    axis.x = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
    axis = Vector2Normalize(axis);

    p.position = Vector2Add(p.position, Vector2Scale(axis, p.speed));

    // Player animation
    frameCounter++;
    if (frameCounter >= (60/10))
    {
        frameCounter = 0;
        currentFrame++;
        
        if (currentFrame > 7) currentFrame = 0;
        frameRec.x = (float)currentFrame*(float)walkAnimation.width/8;
    }

    // Update enemies
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (enemies[i].dead) continue;
        Vector2 toPlayer = Vector2Normalize(Vector2Subtract(p.position, enemies[i].position));
        enemies[i].position = Vector2Add(enemies[i].position, Vector2Scale(toPlayer, enemies[i].speed));
        if (CheckCollisionCircleRec(enemies[i].position, 10, (Rectangle) {p.position.x, p.position.y, 50, 80}))
        {
            p.health -= 1;
            enemies[i].dead = true;
        }
    }

}

void Draw()
{
    //DrawRectangle(p.position.x, p.position.y, 50, 80, BLACK);
    //DrawTexture(walkAnimation, p.position.x, p.position.y, BLACK);
    DrawTextureRec(walkAnimation, frameRec, p.position, WHITE);

    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (!enemies[i].dead) DrawCircleV(enemies[i].position, 10, RED);
    }
    char str[3];
    sprintf(str, "%d", p.health);
    DrawText(str, 10, 10, 32, BLACK);
}

void Unload()
{
    UnloadTexture(walkAnimation);
}