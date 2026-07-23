#include "internal/input.h"
#include "../include/keyboard_mouse_inputs.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Input {

    bool mouseHeld(const Mouse button) {
        GLFWwindow* window = glfwGetCurrentContext();
        return glfwGetMouseButton(window, static_cast<int>(button)) == GLFW_PRESS;
    }

    bool keyHeld(const Key key) {
        GLFWwindow* window = glfwGetCurrentContext();
        return glfwGetKey(window, static_cast<int>(key)) == GLFW_PRESS;
    }
}