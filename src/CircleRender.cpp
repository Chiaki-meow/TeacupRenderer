//
// Created by 程佳雯 on 2022/1/5.
//

#include "CircleRender.h"

CircleRender::CircleRender() {
    initData();
}

CircleRender::~CircleRender() {}

void CircleRender::setShader(Shader shader) {
    this->shader = shader;
}

void CircleRender::calculateCirclePoint(float r, float height) {
    for (int i = 0; i < 100; i++)
    {
        circleVertices[i * 3] = r * cos(i * PI / 50);
        circleVertices[i * 3 + 1] = height;
        circleVertices[i * 3 + 2] = r * sin(i * PI / 50);
    }
}

void CircleRender::initData(){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 300, circleVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CircleRender::Draw() {
    calculateCirclePoint(1, 0);
    initData();
    shader.use();
    glBindVertexArray(VAO);
    glLineWidth(10);
    glDrawArrays(GL_LINE_LOOP, 0, 100);
}

void CircleRender::Draw(float r, float height, const Camera& camera,
                        const glm::vec3 position, const glm::vec3 size, const float rotate, const glm::vec3 axis) {
    calculateCirclePoint(r, height);
    initData();
    shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    // Move origin of rotation to center of quad
    model = glm::translate(model, 0.5f * size);
    // Then rotate
    model = glm::rotate(model, rotate, axis);
    // Move origin back
    model = glm::translate(model, -0.5f * size);
    // Last scale
    model = glm::scale(model, size);
    glm::mat4 view = glm::mat4(camera.GetViewMatrix());
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    glBindVertexArray(VAO);
    glLineWidth(1000);
    glDrawArrays(GL_LINE_LOOP, 0, 100);
}