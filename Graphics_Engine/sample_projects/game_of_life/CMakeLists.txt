add_executable(Game_Of_Life
        gameoflife.cpp
        GOL_helpers.cpp
)

target_link_libraries(Game_Of_Life
        PRIVATE Engine
)
