#ifndef SKYBOX_RENDER_H
#define SKYBOX_RENDER_H

#include "settings.h"

#include "shader.h"
#include "camera.h"
#include "model.h"
#include "stb_image.h"

#include "CubeTexture.h"

using namespace std;

class SkyboxRender{
public:
    int faceID = 0;
    bool faceChange = true;
    SkyboxRender(Shader &_shader);
    ~SkyboxRender();
    void Draw(const CubeTexture& texture, const Camera& camera, const glm::mat4 projection);
    void Draw(const Camera& camera, const glm::mat4 projection);
    unsigned int loadCubemap(vector<std::string> faces);
    void helpChangeSkybox();
private:
    Shader shader;
    unsigned int cubemapTexture;
    vector<std::string> faces;
    vector<std::string> sunsetFaces;
    unsigned int skyboxVAO;
    unsigned int textureID;
    void initRenderData();
};

#endif