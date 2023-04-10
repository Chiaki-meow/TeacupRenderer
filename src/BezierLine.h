//
// Created by 程佳雯 on 2021/12/31.
//

#ifndef GLFWSAMPLE_BEZIERLINE_H
#define GLFWSAMPLE_BEZIERLINE_H

#include "settings.h"
#include "shader.h"
#include "CylinderRender.h"

class BezierLine {
public:
    int bezierID;
    int facesID;
    glm::vec3 position;
    glm::vec3 size;
    glm::vec3 lightPos;
    glm::vec3 specular;
    float shininess;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 lightSpecular;

    BezierLine(){}
    BezierLine(Shader _shader, Shader _bezierShader);
    ~BezierLine();
    void setShader(Shader _shader, Shader _bezierShader){
        shader = _shader;
        bezierShader = _bezierShader;
        init();
    }
    void AddNewPoint(float xPos, float yPos);
    void DeleteAddPoint();
    void Draw();
    void Draw(const glm::vec2& position, glm::mat4 projection);
    void DebugCheck();
    void Clear();
    bool CalculateR(float* & rVertices, float &height);
    void HelpCalculate();
    bool CalculateR();
    void RenderModel(int textureNameIndex, const Camera &camera, const glm::vec3 position, const glm::vec3 size,
                     const float rotate, const glm::vec3 axis, glm::vec3 lightPos, glm::vec3 specular, float shininess, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 lightSpecular);
    void RenderModel(const Camera &camera, const float rotate, const glm::vec3 axis);
    void saveModel();
    void loadModel();
    void saveModel(string filename);
    void loadModel(string filename);

    void setTexture(int faceID);
private:
    int CalCurvePoint();
    void init();
    CylinderRender* cylinderRender;
    Shader shader;
    Shader bezierShader;
    bool startRender;
    bool changeData;
    unsigned int bezierVAO;
    unsigned int bezierVBO;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    unsigned int lineIndices[MAX_INDICES_LEN] = {};
    int lineIndicesLen = 0;

    float bezierVertices[MAX_BEZIER_VERTEX_LEN] = {};
    int bezierVertexLen = 0;

    float lineVertices[MAX_VERTEX_LEN] = {};
    int lineVertexLen = 0;

    float RVertices[MAX_BEZIER_VERTEX_LEN/3] = {};
    float YVertices[MAX_BEZIER_VERTEX_LEN/3] = {};
    int RVertexLen = 0;
    float RVertexHeight = 0;

    vector<string> textureVector;
};

#endif //GLFWSAMPLE_BEZIERLINE_H
