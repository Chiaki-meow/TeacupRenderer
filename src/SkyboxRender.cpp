#include "SkyboxRender.h"

void SkyboxRender::initRenderData() {
    float skyboxVertices[] = {
            // positions
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
    };
    // skybox VAO
    unsigned int skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenTextures(1, &textureID);

    vector<std::string> faces
            {
                    "../resources/textures/skybox/right.jpg",
                    "../resources/textures/skybox/left.jpg",
                    "../resources/textures/skybox/top.jpg",
                    "../resources/textures/skybox/bottom.jpg",
                    "../resources/textures/skybox/front.jpg",
                    "../resources/textures/skybox/back.jpg"
            };
    vector<std::string> sunsetFaces
            {
                    "../resources/textures/container/right.jpg",
                    "../resources/textures/container/left.jpg",
                    "../resources/textures/container/top.jpg",
                    "../resources/textures/container/bottom.jpg",
                    "../resources/textures/container/front.jpg",
                    "../resources/textures/container/back.jpg"
            };
    this->faces = faces;
    this->sunsetFaces = sunsetFaces;
    faceID = 0;
    faceChange = true;
}

void SkyboxRender::Draw(const CubeTexture& texture, const Camera& camera, const glm::mat4 projection) {
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    shader.use();
    glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
    this->shader.setMat4("view", view);
    this->shader.setMat4("projection", projection);

    // skybox cube
    glBindVertexArray(skyboxVAO);
    // bind texture
    glActiveTexture(GL_TEXTURE0);
    texture.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default
}

void SkyboxRender::Draw(const Camera& camera, const glm::mat4 projection) {
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    shader.use();
    glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
    this->shader.setMat4("view", view);
    this->shader.setMat4("projection", projection);
    // skybox cube
    if(faceChange) {
        helpChangeSkybox();
        faceChange = false;
    }
    glBindVertexArray(skyboxVAO);
    // bind texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default
}


SkyboxRender::SkyboxRender(Shader &_shader){
    this->shader = _shader;
    initRenderData();
}

SkyboxRender::~SkyboxRender() {
    glDeleteVertexArrays(1, &this->skyboxVAO);
}

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front)
// -Z (back)
// -------------------------------------------------------


unsigned int SkyboxRender::loadCubemap(vector<std::string> faces){
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

void SkyboxRender::helpChangeSkybox() {
    if(faceID == 0) {
        cubemapTexture = loadCubemap(faces);
    }
    else if(faceID == 1) {
        cubemapTexture = loadCubemap(sunsetFaces);
    }
}
