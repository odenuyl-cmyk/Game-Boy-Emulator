#pragma once

#include <engine.h>
#include <unordered_set>
#include <random>
#include <deque>

struct Point {
    int x;
    int y;

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

struct PointHash {
    size_t operator()(const Point& p) const {
        return (p.x * 73856093) ^ (p.y * 19349663);
    }
};

enum direction { UP, DOWN, LEFT, RIGHT };
inline float step;
inline int x;
inline int y;
inline direction dir;
inline direction prevMove;
inline int resets = 0;

inline std::deque<Point> points;
inline std::unordered_set<Point, PointHash> pointsSet;

Point randomApple(const std::unordered_set<Point, PointHash>& occupied);

void initSnakeGame();

void resetGame();

void drawBoard();