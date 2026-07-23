#include "internal/draw.h"

#include <string>
#include <glad/glad.h>

#include "internal/textures.h"
#include "internal/renderer.h"

namespace Draw {
    rgb_t rgb(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
        float r = float(red) / 255.0f;
        float g = float(green) / 255.0f;
        float b = float(blue) / 255.0f;
        float a= float(alpha) / 255.0f;

        rgb_t result = {r, g, b, a};
        return result;
    }

    void background(rgb_t color) {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void drawTriangle(float x_1, float y_1, float x_2, float y_2, float x_3, float y_3, rgb_t color) {
        float x1 = -1.0f + 2*x_1;
        float x2 = -1.0f + 2*x_2;
        float x3 = -1.0f + 2*x_3;
        float y1 = 1.0f - 2*y_1;
        float y2 = 1.0f - 2*y_2;
        float y3 = 1.0f - 2*y_3;

        Renderer::Vertex v1 = {x1, y1, 0.0f, color.r, color.g, color.b, color.a, 0, 0};
        Renderer::Vertex v2 = {x2, y2, 0.0f, color.r, color.g, color.b, color.a, 0, 0};
        Renderer::Vertex v3 = {x3, y3, 0.0f, color.r, color.g, color.b, color.a, 0, 0};

        Renderer::makeTriangle(v1, v2, v3);
    }

    void drawRect(float x1, float y1, float width, float height, rgb_t color) {
        float x = x1;
        float y = y1 + height;
        drawTriangle(x, y, x + width, y, x, y - height, color);
        drawTriangle(x + width, y, x, y - height, x + width, y - height, color);
    }

    void drawImage(unsigned int textureID, float x1, float y1, float width, float height, rgb_t tint) {
        width *= 2;
        height *= 2;

        float x = -1.0f + 2*x1;
        float y = 1.0f - 2*y1 - height;

        Renderer::Vertex v1 = {x, y, 0, tint.r, tint.g, tint.b, tint.a, 0, 1};
        Renderer::Vertex v2 = {x + width, y, 0, tint.r, tint.g, tint.b, tint.a, 1, 1};
        Renderer::Vertex v3 = {x, y + height, 0, tint.r, tint.g, tint.b, tint.a, 0, 0};
        Renderer::Vertex v4 = {x + width, y + height, 0, tint.r, tint.g, tint.b, tint.a, 1, 0};

        makeImage(textureID, v1, v2, v3, v4);
    }

    unsigned char charIndex(const unsigned char character) {
        unsigned char acc = 0;
        if (character >= 'A' && character <= 'Z') {
            return character - 'A';
        }
        acc += 26;
        if (character >= 'a' && character <= 'z') {
            return character - 'a' + acc;
        }
        acc += 26;
        if (character >= '0' && character <= '9') {
            return character - '0' + acc;
        }

        // hard coded for now!!!
        if (character == ' ') {
            return 90;
        }
        if (character == '!') {
            return 78;
        }
        if (character == ':') {
            return 75;
        }
        if (character == '.') {
            return 80;
        }

        acc += 10;
        return acc;
    }

    void drawChar(const unsigned char character, float x, float y, float sizeFactor, rgb_t color) {
        unsigned char index = charIndex(character);
        float texPosX = (index%13) * 0.0768;
        float texPosY = (index/13) * 0.142857;

        Renderer::Vertex v1 = {x, y, 0, color.r, color.g, color.b, color.a, texPosX, texPosY + 0.142857f};
        Renderer::Vertex v2 = {x + 0.08f/2 * sizeFactor, y, 0, color.r, color.g, color.b, color.a, texPosX + 0.0768f, texPosY + 0.142857f};
        Renderer::Vertex v3 = {x, y + 0.142857f/2 * sizeFactor, 0, color.r, color.g, color.b, color.a, texPosX, texPosY};
        Renderer::Vertex v4 = {x + 0.08f/2 * sizeFactor, y + 0.142857f/2 * sizeFactor, 0, color.r, color.g, color.b, color.a, texPosX + 0.0768f, texPosY};

        makeChar(v1, v2, v3, v4);
    }

    void drawText(const std::string text, const float x1, const float y1, int size, const rgb_t color) {
        float x = -1.0f + 2*x1;
        float y = 1.0f - 2*y1;
        const char* textArray = text.c_str();
        float height = y;
        float sizeFactor = ((float)size)/20.0f;
        float offset = sizeFactor * (0.09/2);
        x -= offset*((float)text.length()/2);
        for (int i = 0; i < text.length(); i++) {
            height = y  + (int(charIndex(textArray[i])/13) * 0.0125f)*sizeFactor/8 - (0.04f + 0.063f*sizeFactor/2);
            drawChar(textArray[i], x + i*offset, height, sizeFactor, color);
        }
    }
}