#include "quadtree.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

struct QuadTreeEntry {
    float x;
    float y;
    int index;
};

int rec_contains(Rectangle b, Vector2 p) {
    return p.x >= b.x &&
    p.x <= b.x + b.width &&
    p.y >= b.y &&
    p.y <= b.y + b.height;
}

int rec_intersects(Rectangle r, Rectangle o) {
    return !(o.x - o.width >=r.x + r.width ||
        o.x + o.width <=r.x - r.width ||
        o.y - o.height >=r.y + r.height ||
        o.y + o.height <=r.y - r.height);
}

Rectangle easy_rec(int x, int y, int w, int h) {
    return (Rectangle) {
        .x = x,
        .y = y,
        .width = w,
        .height = h
    };
}

struct QuadTree make_quadtree(Rectangle b, int cap, int sp) {
    return (struct QuadTree) {
        .boundary = b,
        .cap = cap,
        .points = malloc(sizeof(Vector2) * cap),
        .num_points = 0,
        .divided = 0,
        .show_points = 1,
        .sim_points = sp,
        .northwest = malloc(sizeof(struct QuadTree)),
        .northeast = malloc(sizeof(struct QuadTree)),
        .southwest = malloc(sizeof(struct QuadTree)),
        .southeast = malloc(sizeof(struct QuadTree)),
    };
}

void subdivide(struct QuadTree* qt) {
    Rectangle nw = easy_rec(qt->boundary.x,
        qt->boundary.y, qt->boundary.width/2, qt->boundary.height/2);
    Rectangle ne = easy_rec(qt->boundary.x + qt->boundary.width/2,
        qt->boundary.y, qt->boundary.width/2, qt->boundary.height/2);
    Rectangle se = easy_rec(qt->boundary.x + qt->boundary.width/2,
        qt->boundary.y + qt->boundary.height/2, qt->boundary.width/2, qt->boundary.height/2);
    Rectangle sw = easy_rec(qt->boundary.x,
        qt->boundary.y + qt->boundary.height/2, qt->boundary.width/2, qt->boundary.height/2);

    *qt->northwest = make_quadtree(nw, qt->cap, qt->sim_points);
    qt->northwest->show_points = qt->show_points;

    *qt->northeast = make_quadtree(ne, qt->cap, qt->sim_points);
    qt->northeast->show_points = qt->show_points;

    *qt->southwest = make_quadtree(sw, qt->cap, qt->sim_points);
    qt->southwest->show_points = qt->show_points;

    *qt->southeast = make_quadtree(se, qt->cap, qt->sim_points);
    qt->southeast->show_points = qt->show_points;

    qt->divided = 1;
}

void insert(struct QuadTree* qt, Vector2 p) {
    if (!rec_contains(qt->boundary, p)) {
        return;
    }

    if (qt->num_points < qt->cap) {
        qt->points[qt->num_points] = p;
        qt->num_points++;
    } else {
        if (qt->divided == 0) {
            subdivide(qt);
        }

        insert(qt->northeast, p);
        insert(qt->northwest, p);
        insert(qt->southeast, p);
        insert(qt->southwest, p);
    }
}

void query(struct QTQueryResult* result, Rectangle range, struct QuadTree* qt) {
    if (!rec_intersects(qt->boundary, range)) {
        return;
    } else {
        for (int i = 0; i < qt->num_points; i++) {
            if (rec_contains(range, qt->points[i])) {
                result->results[result->len] = qt->points[i];
                result->len++;
            }
        }
    }

    if (qt->divided) {
        query(result, range, qt->northwest);
        query(result, range, qt->northeast);
        query(result, range, qt->southwest);
        query(result, range, qt->southeast);
    }
}

void show_quadtree(struct QuadTree* qt) {
    DrawRectangleLines(qt->boundary.x, qt->boundary.y, qt->boundary.width, qt->boundary.height, RED);
    if (qt->divided) {
        show_quadtree(qt->northwest);
        show_quadtree(qt->northeast);
        show_quadtree(qt->southwest);
        show_quadtree(qt->southeast);
    }

    if (qt->show_points) {
        for (int i = 0; i < qt->num_points; i++) {
            DrawPixel(qt->points[i].x, qt->points[i].y, WHITE);
        }
    }
}

void free_qt(struct QuadTree* qt) {
    if (qt->northwest != NULL) {
        free_qt(qt->northwest);
    }
    if (qt->northeast != NULL) {
        free_qt(qt->northeast);
    }
    if (qt->southwest != NULL) {
        free_qt(qt->southwest);
    }
    if (qt->southeast != NULL) {
        free_qt(qt->southeast);
    }

    free(qt->northwest);
    free(qt->northeast);
    free(qt->southwest);
    free(qt->southeast);
    free(qt->points);
}
