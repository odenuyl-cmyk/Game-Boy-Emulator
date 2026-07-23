add_executable(Snake
        snake.cpp
        snake_helpers.cpp
        snake_helpers.h
)

target_link_libraries(Snake
        PRIVATE Engine
)