//
// Created by 程佳雯 on 2022/1/6.
//

#ifndef GLFWSAMPLE_CYLINDERRENDER_H
#define GLFWSAMPLE_CYLINDERRENDER_H
#include "settings.h"
#include "shader.h"
#include "camera.h"


class CylinderRender {
public:
    unsigned char *data;
    string filename;
    int lightSet;
    CylinderRender();
    ~CylinderRender();

    void calculateCirclePoint(float top_r, float bot_r, float top_height, float bot_height);

    void calculateModelPoint(float* RVertices, float* YVertices, int RVertexLen);
    void Draw(float r_top, float r_bot, float height, const Camera& camera,
              const glm::vec3 position, const glm::vec3 size, const float rotate = 0.0f, const glm::vec3 axis = glm::vec3(1.0f));
    void Draw(string filename, const Camera &camera, const glm::vec3 position, const glm::vec3 size,
               const float rotate, const glm::vec3 axis, glm::vec3 lightPos, glm::vec3 specular, float shininess, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 lightSpecular);
    void Draw(int fileId, const Camera &camera, const glm::vec3 position, const glm::vec3 size,
               const float rotate, const glm::vec3 axis, glm::vec3 lightPos, glm::vec3 specular, float shininess, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 lightSpecular);
    void setShader(Shader _shader);
    void setTexture(string filename);
    int setTexture(int index);

    void setDiffuseTexture(string filename);
    void setSpecularTexture(string filename);
private:
    Shader shader;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    unsigned int texture;
    unsigned int diffuseMap;
    unsigned int specularMap;

    int circleVertexLen = 0;
    float circleVertices[MAX_CYLINDER_VERTEX_LEN] = {};

    vector<float> CylinderVertex;
    vector<unsigned int> CylinderFaces;


    vector<float> normals;
    vector<float> colors;
    vector<unsigned int> faces;

    vector<float> vertex_textures;
    vector<float> texture_index;

    vector<string> textureFilenameVector;
    vector<unsigned int> textureIdVector;
};


#endif //GLFWSAMPLE_CYLINDERRENDER_H
