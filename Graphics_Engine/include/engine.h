#pragma once

#include <string>
#include "color.h"
#include "keyboard_mouse_inputs.h"

namespace Engine {
    // window-related functions
    void init(int size, std::string windowName);
    void beginFrame();
    void endFrame();
    void frameRate(int frameRate);
    int shouldClose();
    void closeWindow();

    // input-related functions
    double mouseX();
    double mouseY();
    bool mouseHeld(Mouse button);
    bool keyHeld(Key key);

    // texture creation
    unsigned int makeTexture(std::string path);

    // drawing-related functions
    void background(rgb_t color);
    void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, rgb_t color);
    void drawRect(float x, float y, float width, float height, rgb_t color);
    void drawImage(unsigned int textureID, float x, float y, float width, float height, rgb_t tint={1,1,1,1});
    void drawText(std::string text, float x, float y1, int size, rgb_t color);
    rgb_t rgb(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
}