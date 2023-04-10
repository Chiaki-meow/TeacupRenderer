#ifndef DRAW_H
#define DRAW_H

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

#include "point.h"
#include "settings.h"

using namespace std;

class drawMethod{
    Shader drawShader;
    pointManager *point;
    float zBuffer[SCR_WIDTH][SCR_HEIGHT];
    int hw3_flag = 0;
public:
    drawMethod(Shader &shader);
    ~drawMethod(){};
    void drawTriangle(vector<glm::vec3> position, vector<glm::vec3> color, int hw3);
    float drawDepth(int x, int length, int mode);
    void drawHW1(float posx,float posy);
    void drawHW2(float posx,float posy);
    void drawHW3(float posx,float posy);
    void drawHW3Up(float posx,float posy);
};

#endif