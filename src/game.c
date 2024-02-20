#include "raylib.h"
#include "raymath.h"
#include <stdio.h>

typedef struct Animation
{
    int frameCounter;
    int currentFrame;
    Rectangle frameRec;
    bool playing;
} Animation;

typedef struct Mallet
{
    Vector2 position;
    bool whacking;

    Animation animation;
} Mallet;

typedef struct Mole
{
    Vector2 position;
    bool up;
    int timer; // frames till gone

    Animation animation;
} Mole;

Mallet mallet;
Mole moles[5];

Texture2D backgroundTexture;
Texture2D malletTexture;
Texture2D moleTexture;

#define scale 4

int score;
int highscore;
int gameTimer;

void Animate(Animation *animation, int width, int frameCount, int delay)
{
    if (!animation->playing || gameTimer == 0)
    {
        animation->frameRec.x = (float)animation->currentFrame * (float)width / frameCount;
        return;
    }

    animation->frameCounter++;
    if (animation->frameCounter >= delay)
    {
        animation->frameCounter = 0;
        animation->currentFrame++;

        if (animation->currentFrame > frameCount - 1)
        {
            if (frameCount == 4) // if mallet (yes very bizzare logic)
                animation->currentFrame = 0;
            else
                animation->currentFrame = frameCount - 1;
            animation->playing = false;
        }
        animation->frameRec.x = (float)animation->currentFrame * (float)width / frameCount;
    }
}

void Collide()
{
    for (int i = 0; i < 5; i++)
    {
        if (moles[i].up && moles[i].timer == 0 && CheckCollisionPointCircle(mallet.position, Vector2Add(Vector2Scale(moles[i].position, scale), (Vector2){-50, -50}), 50))
        {
            moles[i].animation.playing = false;
            moles[i].animation.currentFrame = 0;
            moles[i].animation.frameCounter = 0;
            moles[i].up = false;
            score++;
            moles[i].timer = GetRandomValue(50,200);
        }
    }
}

void Init()
{
    // Initialize animation
    backgroundTexture = LoadTexture("textures/whack.png");
    malletTexture = LoadTexture("textures/mallet.png");
    moleTexture = LoadTexture("textures/mole.png");

    mallet.animation.frameRec = (Rectangle){0.0f, 0.0f, (float)malletTexture.width / 4, (float)malletTexture.height};
    mallet.animation.frameCounter = 0;
    mallet.animation.currentFrame = 0;
    mallet.animation.playing = false;
    for (int i = 0; i < 5; i++)
    {
        moles[i].animation.frameRec = (Rectangle){0.0f, 0.0f, (float)moleTexture.width / 5, (float)moleTexture.height};
        moles[i].animation.frameCounter = 0;
        moles[i].animation.currentFrame = 0;
        moles[i].animation.playing = true;
    }
    moles[0].position = (Vector2){44, 102};
    moles[1].position = (Vector2){105, 102};
    moles[2].position = (Vector2){166, 102};
    moles[3].position = (Vector2){71, 134};
    moles[4].position = (Vector2){146, 134};

    score = 0;
    highscore = 0;

    gameTimer = 30*30; // depends on frame rate
}

void Update()
{
    if (IsKeyPressed(KEY_R)) Init();
    // Update mallet position
    mallet.position = GetMousePosition();
    mallet.position.x += -90;
    mallet.position.y += -65;

    // Animate sprites
    Animate(&mallet.animation, malletTexture.width, 4, 2);

    // Handle game end
    if (gameTimer == 0)
    {
        for (int i = 0; i < 5; i++) { 
            moles[i].animation.currentFrame = 0;
        }
    } else {
        gameTimer--;
    }
    // Animate moles
    for (int i = 0; i < 5; i++)
    {
        if (moles[i].timer == 0)
        {
           moles[i].animation.playing = true;
        } else {
            moles[i].timer--;
        }
        Animate(&moles[i].animation, moleTexture.width, 5, 3);
        if (!moles[i].animation.playing) moles[i].up = true;
    }

    if (!mallet.animation.playing && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        Collide();
        mallet.animation.playing = true;
    }
}

void Draw()
{
    // DrawRectangle(p.position.x, p.position.y, 50, 80, BLACK);
    DrawTexturePro(
        backgroundTexture,
        (Rectangle){0, 0, backgroundTexture.width, backgroundTexture.height},
        (Rectangle){0, 0, backgroundTexture.width * scale, backgroundTexture.height * scale},
        (Vector2){0, 0},
        0,
        WHITE);
    // DrawTexture(backgroundTexture, 0, 0, WHITE);

    for (int i = 0; i < 5; i++)
    {
        Mole m = moles[i];
        //DrawCircle(moles[i].position.x*scale, moles[i].position.y*scale,50, WHITE);
        DrawTexturePro(
            moleTexture,
            m.animation.frameRec,
            (Rectangle){m.position.x * scale, m.position.y * scale, moleTexture.width / 5 * scale, moleTexture.height * scale},
            (Vector2){0, 0},
            0,
            WHITE);
    }
    char str[3];
    sprintf(str, "%d", score);
    DrawText(str, 275, 260, 32, RED);

    char str2[3];
    sprintf(str2, "%d", gameTimer/30); // depends on frame rate
    DrawText(str2, 615, 260, 32, RED);

    DrawText("WHACK-A-MOLE", 270, 50, 48, GREEN);
    if (gameTimer == 0) DrawText("GAME OVER", 350, 150, 36, RED);

    DrawTexturePro(
        malletTexture,
        mallet.animation.frameRec,
        (Rectangle){mallet.position.x, mallet.position.y, malletTexture.width / 4 * scale, malletTexture.height * scale},
        (Vector2){0, 0},
        0,
        WHITE);
}

void Unload()
{
    UnloadTexture(backgroundTexture);
    UnloadTexture(moleTexture);
    UnloadTexture(malletTexture);
}