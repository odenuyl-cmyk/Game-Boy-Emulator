#pragma once

#include <string>
#include <glad\glad.h>
#include <GLFW/glfw3.h>

namespace Window {
    void init(int size, std::string windowName);
    void beginFrame();
    void endFrame();
    void frameRate(int frameRate);
    int shouldClose();
    void closeWindow();
}