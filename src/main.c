#include <stdio.h>
#include <raylib.h>
#include <string.h>
#include <raymath.h>

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
};

struct Player {
    Vector2 position;
    float angle;
    Vector2 direction;
};

static struct GameState {
    struct Node nodes[NODE_COUNT];
    Vector2 playfieldSize;
    struct Player player;
} game;

void initialize(void)
{
    game.playfieldSize = (Vector2){800, 450};

    for (int i = 0; i < NODE_COUNT; i++)
    {
        game.nodes[i].alignment = GetRandomValue(ALIGNMENT_NEUTRAL, ALIGNMENT_EVIL);
        game.nodes[i].position = (Vector2){GetRandomValue(0, game.playfieldSize.x), GetRandomValue(0, game.playfieldSize.y)};
    }

    game.player.position = (Vector2){game.playfieldSize.x / 2, game.playfieldSize.y / 2};
}

void update(void)
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
        game.player.angle -= 0.01;
    }
    else if (IsKeyDown(KEY_RIGHT))
    {
        game.player.angle += 0.01;
    }

    // draw
    ClearBackground(BLACK);

    BeginDrawing();

        DrawPoly(game.player.position, 6, 25, game.player.angle*RAD2DEG, BLUE);
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
            DrawCircleLinesV(game.nodes[i].position, 30, nodeColor);

            game.nodes[i].position.x -= 0.5;
            if (game.nodes[i].position.x < -35)
            {
                game.nodes[i].position = (Vector2){game.playfieldSize.x + 35, GetRandomValue(0, game.playfieldSize.y)};
            }
        }

    EndDrawing();
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
