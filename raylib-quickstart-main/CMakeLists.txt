cmake_minimum_required(VERSION 3.21)
project(MyRaylibGame CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Pull raylib using CMake's FetchContent
include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        master       # Or specify a version tag like 5.5
)
FetchContent_MakeAvailable(raylib)

# 2. Create your executable
add_executable(${PROJECT_NAME} 
    src/main.cpp 
    src/manager.cpp
    src/map.cpp
    src/building.cpp
    src/character.cpp
    src/ball.cpp
)
# 3. Link raylib to your executable
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# 4. Platform-specific configurations (Crucial for macOS & Windows)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
elseif(WIN32)
    # Ensures console window doesn't pop up on Windows release builds (Optional)
    # set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
endif()