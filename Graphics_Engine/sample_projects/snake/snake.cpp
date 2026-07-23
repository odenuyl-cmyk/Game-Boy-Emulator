#include <engine.h>
#include "snake_helpers.h"
#include <deque>
#include <unordered_set>
#include <random>

using namespace std;
using namespace Engine;

int main() {
    init(1000, "Snake Game");
    frameRate(140);

    initSnakeGame();
    Point apple = randomApple(pointsSet);

    int counter = 0;

    auto appleImage = makeTexture("assets/textures/apple.png");

    while (!shouldClose()) {
        beginFrame();

        if (keyHeld(Key::W) && dir != DOWN && prevMove != DOWN) {
            dir = UP;
        } if (keyHeld(Key::A) && dir != RIGHT && prevMove != RIGHT) {
            dir = LEFT;
        } if (keyHeld(Key::S) && dir != UP && prevMove != UP) {
            dir = DOWN;
        } if (keyHeld(Key::D) && dir != LEFT && prevMove != LEFT) {
            dir = RIGHT;
        }

        if (counter % 20 == 0) {
            if (dir == UP) {
                y -= 1;
                prevMove = UP;
            } else if (dir == DOWN) {
                y += 1;
                prevMove = DOWN;
            } else if (dir == LEFT) {
                x -= 1;
                prevMove = LEFT;
            } else if (dir == RIGHT) {
                x += 1;
                prevMove = RIGHT;
            }
            if (x < 0 || x >= 25) {
                resetGame();
                continue;
            }
            if (y < 0 || y >= 23) {
                resetGame();
                continue;
            }
            points.push_back({x, y});
            pointsSet.insert({x, y});

            if (x == apple.x && y == apple.y) {
                apple = randomApple(pointsSet);
            } else {
                pointsSet.erase(points.front());
                points.pop_front();
            }

            if (pointsSet.size() != points.size()) {
                resetGame();
                continue;
            }
        }

        drawBoard();

        for (Point p : points) {
            drawRect(p.x*step, p.y*step, step, step, Color::GREEN);
        }
        
        drawImage(appleImage, apple.x*step, apple.y*step, step, step);
        string scoreText = "Score: " + to_string(points.size() - 3);
        string resettext = "Resets: " + to_string(resets);
        drawText("Eat apples to grow!", 0.25, 0.95, 20, Color::YELLOW);
        drawText(scoreText, 0.59, 0.95, 20, Color::VIOLET);
        drawText(resettext, 0.86, 0.95, 20, Color::INDIGO);

        endFrame();
        counter++;
    }

    closeWindow();

    return counter;
}