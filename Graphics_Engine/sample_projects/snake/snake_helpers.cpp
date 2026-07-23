#include "snake_helpers.h"


Point randomApple(const std::unordered_set<Point, PointHash>& occupied) {
    static std::random_device rand;
    static std::mt19937 generate(rand());
    static std::uniform_int_distribution<int> dist(0, 24);

    Point apple;

    do {
        apple.x = dist(generate);
        apple.y = dist(generate)%22;
    } while (occupied.contains(apple)); // C++20

    return apple;
}

void initSnakeGame() {
    dir = RIGHT;
    prevMove = RIGHT;
    step = 0.04;

    x = 10;
    y = 10;

    points.push_back({8, 10});
    pointsSet.insert({8, 10});
    points.push_back({9, 10});
    pointsSet.insert({9, 10});
    points.push_back({10, 10});
    pointsSet.insert({10, 10});
}

void resetGame() {
    points.clear();
    pointsSet.clear();
    resets += 1;

    initSnakeGame();
}

void drawBoard() {
    for (int j = 0; j < 23; j++) {
        for (int i = 0; i < 13; i++) {
            Engine::drawRect(i*0.08 + 0.04*(j%2), j*0.04, .04, .04, Engine::rgb(25, 25, 25, 255));
        }
    }
}