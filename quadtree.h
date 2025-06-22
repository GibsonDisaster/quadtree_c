#ifndef QUADTREE
#define QUADTREE

#include <raylib.h>

struct QuadTreeEntry {
    Vector2 pos;
    int index;
};

struct QuadTree {
    Rectangle boundary;
    int cap;
    struct QuadTreeEntry* points;
    int num_points;
    int sim_points;
    struct QuadTree* northwest;
    struct QuadTree* northeast;
    struct QuadTree* southwest;
    struct QuadTree* southeast;
    int divided;
    int show_points;
};

struct QTQueryResult {
    int len;
    struct QuadTreeEntry* results;
};

int rec_contains(Rectangle b, Vector2 p);
Rectangle easy_rec(int x, int y, int w, int h);

struct QuadTree make_quadtree(Rectangle b, int cap, int sp);
void subdivide(struct QuadTree* qt);
void insert(struct QuadTree* qt, struct QuadTreeEntry qte);
void query(struct QTQueryResult* result, Rectangle range, struct QuadTree* qt);
void show_quadtree(struct QuadTree* qt);

void free_qt(struct QuadTree* qt);
#endif
