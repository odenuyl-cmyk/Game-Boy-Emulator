#pragma once


struct rgb_t {
    float r, g, b, a;
};

// creates color shortcuts without rgb function step, slightly more efficient
namespace Engine::Color {
    inline constexpr rgb_t RED = {1, 0, 0, 1};
    inline constexpr rgb_t ORANGE = {1, .463, 0, 1};
    inline constexpr rgb_t YELLOW = {1, 1, 0, 1};
    inline constexpr rgb_t GREEN = {0, 1, 0, 1};
    inline constexpr rgb_t BLUE = {0, 0, 1, 1};
    inline constexpr rgb_t INDIGO = {.247, 0, 1, 1};
    inline constexpr rgb_t VIOLET = {.69, 0, 1, 1};
    inline constexpr rgb_t WHITE = {1, 1, 1, 1};
    inline constexpr rgb_t BLACK = {0, 0, 0, 1};
    inline constexpr rgb_t GREY = {.5, .5, .5, 1};
}