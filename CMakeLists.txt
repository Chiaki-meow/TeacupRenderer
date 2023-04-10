cmake_minimum_required(VERSION 3.17)
project(GlfwSample)

set(CMAKE_CXX_STANDARD 11)

# imgui
include_directories(
        src/imgui
        src/imgui/backends
)

file(GLOB SRC
        src/imgui/*.h
        src/imgui/*.cpp
        )

file (GLOB PLATFORM_SRC
        src/imgui/backends/imgui_impl_glfw.*
        src/imgui/backends/imgui_impl_opengl3.*
        )

# glfw
option(GLFW_BUILD_TESTS OFF)
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
add_subdirectory(External/glfw-master)
include_directories(External/glfw-master/include)
LIST(APPEND LIBS glfw ${GLFW_LIBRARIES})

# glad
set(GLAD_SRC External/glad-c/src/glad.c)
if(NOT_WIN32)
    LIST(APPEND LIBS dl)
endif()
include_directories(External/glad-c/include)

# glm
include_directories(External/glm-0.9.9.7)

# assimp
option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_TESTS OFF)
option(INJECT_DEBUG_POSTFIX OFF)  #on if debug
add_subdirectory(External/assimp-5.0.1)
include_directories(External/assimp-5.0.1/include)
LIST(APPEND LIBS assimp)

# ours
include_directories(include)
file(GLOB SOURCE_FILES
    "include/*.h"
    "src/*.cpp" "src/SpriteRenderer.h" "src/SpriteRenderer.cpp" "src/CylinderRender.*"
        "src/ParticleSystem/*.cpp"
)

add_executable(GlfwSample ${SOURCE_FILES} ${GLAD_SRC} ${SRC} ${PLATFORM_SRC})
target_link_libraries(GlfwSample ${LIBS})