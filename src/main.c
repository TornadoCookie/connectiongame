#include <stdio.h>
#include <raylib.h>
#include <string.h>
#include <raymath.h>

#define BOXITO_IMPLEMENTATION
#include "boxito.h"

//#if PLATFORM == web
    //#include <emscripten/emscripten.h>
//#endif

typedef void (*em_callback_func)(void);
void emscripten_set_main_loop(em_callback_func func, int fps, bool simulate_infinite_loop);

#define NODE_COUNT 50

enum NodeAlignment {
    ALIGNMENT_NEUTRAL,
    ALIGNMENT_GOOD,
    ALIGNMENT_EVIL
};

struct Node {
    enum NodeAlignment alignment;
    Vector2 position;
    int respawnTimer;
};

struct Player {
    Vector2 position;
    float angle;
    Vector2 direction;
    float health;
    int exp;
};

enum GameScreen {
    SCREEN_MENU,
    SCREEN_GAME,
    SCREEN_END
};

static struct GameState {
    struct Node nodes[NODE_COUNT];
    Vector2 playfieldSize;
    struct Player player;
    enum GameScreen screen;
} game;

void initialize(void)
{
    game.playfieldSize = (Vector2){800, 450};

    for (int i = 0; i < NODE_COUNT; i++)
    {
        //game.nodes[i].alignment = GetRandomValue(ALIGNMENT_NEUTRAL, ALIGNMENT_EVIL);
        //game.nodes[i].position = (Vector2){GetRandomValue(0, game.playfieldSize.x), GetRandomValue(0, game.playfieldSize.y)};
        game.nodes[i].respawnTimer = GetRandomValue(0, 2048);
    }

    game.player.position = (Vector2){game.playfieldSize.x / 2, game.playfieldSize.y / 2};
    game.player.health = 120;
}

#define PLAYER_RADIUS 25
#define NODE_RADIUS 30
#define APPARENT_NODE_RADIUS NODE_RADIUS+PLAYER_RADIUS/2

void game_update(void)
{
    // update
    game.player.direction = Vector2Rotate((Vector2){0, 1}, game.player.angle);

    if (IsKeyDown(KEY_DOWN))
    {
        game.player.position = Vector2Subtract(game.player.position, Vector2Multiply(game.player.direction, (Vector2){2, 2}));
    }
    else if (IsKeyDown(KEY_UP))
    {
        game.player.position = Vector2Add(game.player.position, Vector2Multiply(game.player.direction, (Vector2){2, 2}));
    }

    if (IsKeyDown(KEY_LEFT))
    {
        game.player.angle -= 0.05;
    }
    else if (IsKeyDown(KEY_RIGHT))
    {
        game.player.angle += 0.05;
    }

    for (int i = 0; i < NODE_COUNT; i++)
    {
        game.nodes[i].position.x -= 0.5;
        if (game.nodes[i].position.x < -NODE_RADIUS)
        {
            game.nodes[i].respawnTimer--;
            if (game.nodes[i].respawnTimer <= 0)
            {
                game.nodes[i].position = (Vector2){game.playfieldSize.x + NODE_RADIUS, GetRandomValue(0, game.playfieldSize.y)};
                game.nodes[i].alignment = GetRandomValue(ALIGNMENT_NEUTRAL, ALIGNMENT_EVIL);
                game.nodes[i].respawnTimer = GetRandomValue(0, 2048);
            }
        }

        if (CheckCollisionCircles(game.nodes[i].position, APPARENT_NODE_RADIUS, game.player.position, PLAYER_RADIUS))
        {
            switch (game.nodes[i].alignment)
            {
                case ALIGNMENT_GOOD: game.player.health++; break;
                case ALIGNMENT_EVIL: game.player.health-=2; break;
                case ALIGNMENT_NEUTRAL: game.player.exp++; break;
            }
        }
    }

    // draw
    ClearBackground(BLACK);

    BeginDrawing();

        DrawPoly(game.player.position, 6, PLAYER_RADIUS, game.player.angle*RAD2DEG, BLUE);
        DrawLineV(game.player.position, Vector2Add(game.player.position, Vector2Multiply(game.player.direction, (Vector2){30, 30})), WHITE);

        for (int i = 0; i < NODE_COUNT; i++)
        {
            Color nodeColor;
            switch (game.nodes[i].alignment)
            {
                case ALIGNMENT_NEUTRAL: nodeColor = WHITE; break;
                case ALIGNMENT_GOOD: nodeColor = GREEN; break;
                case ALIGNMENT_EVIL: nodeColor = RED; break;
            }
            DrawCircleV(game.nodes[i].position, 5, nodeColor);
            DrawCircleLinesV(game.nodes[i].position, APPARENT_NODE_RADIUS, nodeColor);

            if (CheckCollisionCircles(game.nodes[i].position, NODE_RADIUS, game.player.position, PLAYER_RADIUS))
            {
                DrawLineV(game.nodes[i].position, game.player.position, nodeColor);
            }
        }

        DrawText(TextFormat("HEALTH: %f\n", game.player.health), 0, 0, 20, WHITE);
        DrawText(TextFormat("EXP: %d\n", game.player.exp), 0, 20, 20, WHITE);

    EndDrawing();
}

void menu_update(void)
{
    // update

    // draw
    BeginDrawing();
        ClearBackground(BLACK);
        DrawTextCentered("Connections game in space(?)", 30, 30, WHITE);

        if (DrawButtonCentered("Play", GREEN, WHITE, DARKGREEN, LIGHTGRAY, 70))
        {
            game.screen = SCREEN_GAME;
        }

    EndDrawing();
}

void end_update(void)
{

}

void update(void)
{
    switch (game.screen)
    {
        case SCREEN_MENU: menu_update(); break;
        case SCREEN_GAME: game_update(); break;
        case SCREEN_END: end_update(); break;
    }
}

int main()
{
    InitWindow(800, 450, "Hello world");

    initialize();
    SetTargetFPS(60);

if (!strcmp(PLATFORM, "web")) //#if PLATFORM == web
{
    emscripten_set_main_loop(update, 60, 1);
}
else//#else
{
    while (!WindowShouldClose())
    {
        update();
    }

}//#endif

    return 0;
}
