//
// Created by 程佳雯 on 2022/1/5.
//

#ifndef CIRCLERENDER_H
#define CIRCLERENDER_H
#include "settings.h"
#include "shader.h"
#include "Texture2D.h"
#include "camera.h"

class CircleRender {
public:
    CircleRender();
    ~CircleRender();

    void calculateCirclePoint(float r, float height);
    void Draw();
    void Draw(float r, float height, const Camera& camera,
                            const glm::vec3 position, const glm::vec3 size, const float rotate = 0.0f, const glm::vec3 axis = glm::vec3(1.0f));
    void setShader(Shader shader);

private:
    Shader shader;
    unsigned int VAO;
    unsigned int VBO;

    void initData();
    int circleVertexLen = 0;
    float circleVertices[MAX_CIRCLE_VERTEX_LEN] = {};

};


#endif //GLFWSAMPLE_CIRCLERENDER_H
