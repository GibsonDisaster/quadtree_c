#include "quadtree.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    Vector2* points;
    int num_points;
} Sim;

Sim make_sim(int n) {
    Vector2* pts = malloc(sizeof(Vector2) * n);

    return (Sim) {
        .points = pts,
        .num_points = n
    };
}

int main(void) {
    srand(time(NULL));

    InitWindow(800, 800, "quadtree");
    SetTargetFPS(60);

    HideCursor();

    Rectangle mouseBoundary = easy_rec(GetRandomValue(0, 800), GetRandomValue(0, 800), 100, 100);
    Rectangle boundary = easy_rec(0, 0, 800, 800);
    struct QuadTree qt = make_quadtree(boundary, 20, 100);

    for (int i = 0; i < 100; i++) {
        insert(&qt, (Vector2) {.x = GetRandomValue(0, 800), .y = GetRandomValue(0, 800)});
    }

    while (!WindowShouldClose())
    {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            insert(&qt, (Vector2) {.x = GetMouseX(), .y = GetMouseY()});
        }

        mouseBoundary.x = GetMouseX() - 50;
        mouseBoundary.y = GetMouseY() - 50;

        BeginDrawing();
        ClearBackground(BLACK);

        show_quadtree(&qt);

        DrawRectangleLinesEx(mouseBoundary, 1.0, GREEN);

        struct QTQueryResult qt_result = (struct QTQueryResult) {
            .results = malloc(sizeof(Vector2) * qt.sim_points),
            .len = 0
        };
        query(&qt_result, mouseBoundary, &qt);

        if (qt_result.len > 0) {
            for (int i = 0; i < qt_result.len; i++) {
                DrawCircle(qt_result.results[i].x, qt_result.results[i].y, 1.0, GREEN);
            }
        }

        free(qt_result.results);

        EndDrawing();
    }

    free_qt(&qt);

    CloseWindow();
}
