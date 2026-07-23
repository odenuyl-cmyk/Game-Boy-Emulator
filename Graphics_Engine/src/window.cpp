#include "internal/window.h"
#include "internal/input.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <thread>
#include <chrono>
#include "internal/renderer.h"

using timeStamp = std::chrono::high_resolution_clock;
using duration = std::chrono::duration<double>;

namespace Window {
    // initiate window and frame logic
    static GLFWwindow* window = nullptr;
    static auto timePerFrame = duration(1.0 / 60.0);
    static timeStamp::time_point nextFrameTime = timeStamp::now();

    // tells OpenGL where screen is upon resize
    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        int size = std::min(width, height);
        int xOff = (width - size) / 2;
        int yOff = (height - size) / 2;
        glViewport(xOff, yOff, size, size);
    }

    void useCustomCursor() {
        unsigned char pixels[16 * 16 * 4];
        memset(pixels, 0xf0, sizeof(pixels));

        GLFWimage image;
        image.width = 16;
        image.height = 16;
        image.pixels = pixels;

        GLFWcursor* cursor = glfwCreateCursor(&image, 0, 0);
        glfwSetCursor(window, cursor);
    }

    // function to set everything up: takes width and height and does everything else automatically
    void init(int size, const std::string windowName) {

        // throw error if initialization fails
        if (!glfwInit())
            throw std::runtime_error("GLFW initialization failed");

        // tell window which properties to obey
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
        glfwWindowHint(GLFW_FLOATING, GL_TRUE);
        glfwWindowHint(GLFW_POSITION_X, 800);
        glfwWindowHint(GLFW_POSITION_Y, 200);

        // create the actual window and throw error if this fails
        window = glfwCreateWindow(size, size, windowName.c_str(), nullptr, nullptr);
        if (window == nullptr) throw std::runtime_error("Window creation failed");

        // set the current window and assign resizing function
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        // set up custom cursor and how it should be used
        // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
        // useCustomCursor();

        // vsync toggle: 0 (off), 1 (on)
        glfwSwapInterval(0);

        // load GLAD (renderer) and set intial window size
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            throw std::runtime_error("Failed to initialize GLAD\n");
        }
        glViewport(0, 0, size, size);
        //glfwSetWindowAspectRatio(window, 1, 1);

        Renderer::init();

    }

    // abstraction of pollEvents; call at beginning of rendering loop
    void beginFrame() {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwGetCursorPos(window, &Input::mouseXPos, &Input::mouseYPos);
        glfwPollEvents();
    }

    // resets vertices and swaps the buffers; call at the end of rendering loop
    void endFrame() {
        Renderer::render();
        glfwSwapBuffers(window);

        // frame rate logic, sleeps for remaining time and accounts for lag
        auto now = timeStamp::now();
        if (now > nextFrameTime) {
            nextFrameTime = now;
        } else {
            std::this_thread::sleep_until(nextFrameTime);
        }

        nextFrameTime += std::chrono::duration_cast<timeStamp::duration>(timePerFrame);
    }

    // Set custom frame rate
    void frameRate(int frameRate) {
        if (frameRate <= 0) {
            throw std::runtime_error("Frame rate must be positive!");
        }
        timePerFrame = duration(1.0 / frameRate);
    }

    int shouldClose() {
        return glfwWindowShouldClose(window);
    }

    // Clean up and terminate program
    void closeWindow() {
        Renderer::close();
        glfwTerminate();
    }
}