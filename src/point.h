#ifndef POINT_H
#define POINT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "shader.h"
#include "camera.h"
#include "model.h"
#include "stb_image.h"

using namespace std;

typedef struct { float x, y; } vector_t;
typedef vector_t point_t;//顶点
typedef struct { float r, g, b,a; } color_t;//颜色

class pointManager{
    Shader ourShader;
    unsigned int VAO;

public:
    pointManager(Shader &shader);
    ~pointManager();
    void initPoint();
    void drawPoint(const glm::vec2& position, const glm::vec3 color);
};

#endif