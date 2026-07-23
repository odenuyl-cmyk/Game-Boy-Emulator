#pragma once

namespace Renderer {
    struct Vertex {
        float x, y, z;
        float r, g, b, a;
        float u, v;
    };
    void init();
    void makeTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3);
    void makeImage(unsigned int textureID, const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vertex& v4);
    void makeChar(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vertex& v4);
    void render();
    void close();
}