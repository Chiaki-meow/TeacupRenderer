#ifndef CUBE_RENDER_H
#define CUBE_RENDER_H

#include "settings.h"

#include "shader.h"
#include "camera.h"
#include "model.h"
#include "stb_image.h"
#include "Texture2D.h"

using namespace std;

class CubeRender{
    Shader shader;
    unsigned int cubeVAO;
    void initRenderData();
public:
    CubeRender(Shader &_shader);
    ~CubeRender();
    void Draw(const Texture2D& texture, const Camera& camera,
              const glm::vec3 position, const glm::vec3 size, const float rotate, const glm::vec3 axis);
};

#endif