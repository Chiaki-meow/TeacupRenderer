#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>
#include "point.h"

using namespace std;

pointManager::pointManager(Shader &shader){
    this->ourShader = shader;
    initPoint();

}

pointManager::~pointManager(){
    glDeleteVertexArrays(1, &VAO);
}

void pointManager::initPoint(){
    float vertices[]={
        0.0f,0.0f
    };

    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 位置属性
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 颜色属性
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    // glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);
}

void pointManager::drawPoint(const glm::vec2& position, const glm::vec3 color){
    this->ourShader.use();

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));

    this->ourShader.setMat4("model", model);
	this->ourShader.setVec3("uniColor", color);

    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glPointSize(2);
    glDrawArrays(GL_POINTS, 0, 1);
    glBindVertexArray(0);
}

// void pointManager::drawVAO(){
//     // render
//     glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
//     glDrawArrays(GL_POINTS, 0, 1);
// }