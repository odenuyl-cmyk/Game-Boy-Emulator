#include "internal/renderer.h"

#include <filesystem>

#include "internal/window.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "internal/textures.h"

namespace Renderer {

    static unsigned int shaderProgram = 0;
    static unsigned int VAO = 0;
    static unsigned int VBO = 0;

    static Vertex* vertices = nullptr;
    static int vertexCount = 0;
    static const unsigned int MAX_VERTICES = 20000;

    static unsigned int currentTexture = 0;
    static unsigned int defaultTexture = 0;
    static unsigned int pixelText = 0;
    static std::string pixelTextPath =  "assets/fonts/pixeltextmap.png";
    static std::string vertexShaderPath = "assets/shaders/vertex.shader";
    static std::string fragmentShaderPath = "assets/shaders/fragment.shader";

    // create vertex/fragment shaders from another file
    static std::string shaderFromFile(const std::string& filePath) {
        std::ifstream stream(filePath);
        std::string line;
        std::string result;

        while (std::getline(stream, line)) {
            result += line + "\n";
        }

        return result;
    }

    static unsigned int compileShader(unsigned int type, const std::string& source) {
        unsigned int id = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        int status;
        glGetShaderiv(id, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE) {
            int length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            char* message = (char*) alloca(length * sizeof(char));
            glGetShaderInfoLog(id, length, &length, message);
            std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
            std::cout << message << std::endl;
            glDeleteShader(id);
            return 0;
        }

        return id;
    }

    static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader) {
        unsigned int program = glCreateProgram();
        unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
        unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        glValidateProgram(program);

        glDetachShader(program, vs);
        glDetachShader(program, fs);
        glDeleteShader(vs);
        glDeleteShader(fs);

        return program;
    }

    void init() {
        std::string vertexSource = shaderFromFile(vertexShaderPath);
        std::string fragmentSource = shaderFromFile(fragmentShaderPath);
        shaderProgram = createShader(vertexSource, fragmentSource);

        // set default texture and allow alpha channel to work for opacity logic
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glGenTextures(1, &defaultTexture);
        glBindTexture(GL_TEXTURE_2D, defaultTexture);

        unsigned char white[] = { 255, 255, 255, 255 };
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,1, 1,0,GL_RGBA,GL_UNSIGNED_BYTE, white);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        pixelText = Texture::makeTexture(pixelTextPath);

        // set up vertex array with fixed capacity:
        // triangle: 3 vertices, 3 array slots
        // rectangle: 4 vertices, 6 array slots
        // image: 4 vertices, 6 array slots
        vertices = new Vertex[MAX_VERTICES];

        // initialize GPU buffers
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, MAX_VERTICES * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

        // tell GPU how to read and separate vertices
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(7 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    // internal logic for drawing untextured triangles, called in draw.cpp
    void makeTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3)
    {
        if (currentTexture != defaultTexture) {
            render();
            currentTexture = defaultTexture;
        }

        vertices[vertexCount++] = v1;
        vertices[vertexCount++] = v2;
        vertices[vertexCount++] = v3;
    }

    // internal logic for drawing textured quads, called in draw.cpp
    void makeImage(unsigned int textureID, const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vertex& v4)
    {
        if (currentTexture != textureID) {
            render();
            currentTexture = textureID;
        }

        vertices[vertexCount++] = v1;
        vertices[vertexCount++] = v2;
        vertices[vertexCount++] = v3;
        vertices[vertexCount++] = v2;
        vertices[vertexCount++] = v3;
        vertices[vertexCount++] = v4;
    }

    void makeChar(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vertex& v4) {
        makeImage(pixelText, v1, v2, v3, v4);
    }

    // called when frame ends or texture changes; draws everything in the VAO and resets it
    void render()
    {
        if (vertexCount == 0) return;

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0,vertexCount * sizeof(Vertex), vertices);
        glUseProgram(shaderProgram);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, currentTexture);

        const int uTexturelocation = glGetUniformLocation(shaderProgram, "uTexture");
        glUniform1i(uTexturelocation, 0);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);

        vertexCount = 0;
    }

    void close() {
        delete[] vertices;
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
        glDeleteProgram(shaderProgram);
    }
}
