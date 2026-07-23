#pragma once

#include "internal/draw.h"
#include "internal/input.h"
#include "internal/textures.h"
#include "internal/window.h"
#include "keyboard_mouse_inputs.h"

namespace Engine {
    // window related functions
    void init(int size, std::string windowName) {
        Window::init(size, windowName);
    }

    void beginFrame() {
        Window::beginFrame();
    }

    void endFrame() {
        Window::endFrame();
    }

    void frameRate(int frameRate) {
        Window::frameRate(frameRate);
    }

    int shouldClose() {
        return Window::shouldClose();
    }

    void closeWindow() {
        Window::closeWindow();
    }


    // input-related functions
    double mouseX() {
        return Input::mouseXPos;
    }

    double mouseY() {
        return Input::mouseYPos;
    }

    bool mouseHeld(Mouse button) {
        return Input::mouseHeld(button);
    }

    bool keyHeld(Key key) {
        return Input::keyHeld(key);
    }

    // texture creation
    unsigned int makeTexture(const std::string path) {
        return Texture::makeTexture(path);
    }

    // drawing related functions
    void background(rgb_t color) {
        Draw::background(color);
    }

    void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, rgb_t color) {
        Draw::drawTriangle(x1, y1, x2, y2, x3, y3, color);
    }

    void drawRect(float x, float y, float width, float height, rgb_t color) {
        Draw::drawRect(x, y, width, height, color);
    }

    void drawImage(unsigned int textureID, float x, float y, float width, float height, rgb_t tint={1,1,1,1}) {
        Draw::drawImage(textureID, x, y, width, height, tint);
    }

    void drawText(const std::string text, float x, float y, int size, rgb_t color) {
        Draw::drawText(text, x, y, size, color);
    }

    rgb_t rgb(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
        return Draw::rgb(red, green, blue, alpha);
    }

}