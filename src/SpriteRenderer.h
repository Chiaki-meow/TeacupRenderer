//
// Created by 程佳雯 on 2022/1/4.
//

#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include "settings.h"
#include "Texture2D.h"
#include "shader.h"
#include "camera.h"


class SpriteRenderer
{
public:
    unsigned char *data;
    // Constructor (inits shaders/shapes)
    SpriteRenderer(Shader &shader);
    // Destructor
    ~SpriteRenderer();
    // Renders a defined quad textured with given sprite
    void DrawSprite(string filename, glm::vec2 position, glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
    void Draw3DSprite(string filename, const Camera &camera, const glm::vec3 position, const glm::vec3 size,
    const float rotate, const glm::vec3 axis);
private:
    // Render state
    Shader shader;
    unsigned int quadVAO;
    unsigned int VBO;
    unsigned int texture;

    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();
    void setTexture(string filename);
};

#endif