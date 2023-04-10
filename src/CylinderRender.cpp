//
// Created by 程佳雯 on 2022/1/6.
//

#include "CylinderRender.h"

CylinderRender::CylinderRender() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    filename = "../resources/textures/china2.jpg";
    lightSet = 0;

    glGenTextures(1, &texture);

    this->textureFilenameVector.emplace_back("../resources/textures/wood1.jpg");
    this->textureFilenameVector.emplace_back("../resources/textures/wood2.jpg");
    this->textureFilenameVector.emplace_back("../resources/textures/stone1.jpg");
    this->textureFilenameVector.emplace_back("../resources/textures/stone2.jpg");
    this->textureFilenameVector.emplace_back("../resources/textures/china1.jpg");
    this->textureFilenameVector.emplace_back("../resources/textures/china2.jpg");

    for(int i=0;i<textureFilenameVector.size();i++){
        textureIdVector.push_back(setTexture(i));
    }

}

CylinderRender::~CylinderRender() {}

void CylinderRender::calculateCirclePoint(float top_r, float bot_r, float top_height, float bot_height) {
    //top
    faces.clear();
    circleVertexLen=0;

    int num_samples = 100;

    for (int i = 0; i < 3*num_samples; i+=3)
    {
        circleVertices[i] = (top_r * cos(i * PI / (num_samples/2)));
        circleVertices[i+1]=(top_height);
        circleVertices[i+2]=(top_r * sin(i * PI / (num_samples/2)));
        circleVertexLen+=3;

        normals.push_back(top_r * cos(i * PI / (num_samples/2)));
        normals.push_back(0);
        normals.push_back(top_r * sin(i * PI / (num_samples/2)));

//        colors.push_back(top_r * cos(i * PI / (num_samples/2)));
//        colors.push_back(0);
//        colors.push_back(top_r * sin(i * PI / (num_samples/2)));
    }
    //bottom
    for (int i = 0; i < 3*num_samples; i+=3)
    {
        circleVertices[300+i] = (bot_r * cos(i * PI / (num_samples/2)));
        circleVertices[300+i+1]=(bot_height);
        circleVertices[300+i+2]=(bot_r * sin(i * PI / (num_samples/2)));
        circleVertexLen+=3;

        normals.push_back(bot_r * cos(i * PI / (num_samples/2)));
        normals.push_back(0);
        normals.push_back(bot_r * sin(i * PI / (num_samples/2)));

//        colors.push_back(top_r * cos(i * PI / (num_samples/2)));
//        colors.push_back(0);
//        colors.push_back(top_r * sin(i * PI / (num_samples/2)));
    }

    for (int i = 0; i < num_samples; i++)
    {
        // 面片1
        faces.push_back(i);
        faces.push_back((i + 1) % num_samples);
        faces.push_back((i) + num_samples);

        // 面片1对应的顶点的纹理坐标
        vertex_textures.push_back(1.0 * i / num_samples);
        vertex_textures.push_back(0.0);
        vertex_textures.push_back(1.0 * (i+1) / num_samples);
        vertex_textures.push_back(0.0);
        vertex_textures.push_back(1.0 * i / num_samples);
        vertex_textures.push_back(1.0);
        // 对应的三角面片的纹理坐标的下标
        texture_index.push_back(6*i);
        texture_index.push_back(6*i+1);
        texture_index.push_back(6*i+2);

        // 面片2
        faces.push_back((i) + num_samples);
        faces.push_back((i + 1) % num_samples);
        faces.push_back((i + num_samples + 1) % (num_samples) + num_samples);

        // 面片2对应的顶点的纹理坐标
        vertex_textures.push_back(1.0 * i / num_samples);
        vertex_textures.push_back(1.0);
        vertex_textures.push_back(1.0 * (i+1) / num_samples);
        vertex_textures.push_back(0.0);
        vertex_textures.push_back(1.0 * (i+1) / num_samples);
        vertex_textures.push_back(1.0);
        // 对应的三角面片的纹理坐标的下标
        texture_index.push_back(6*i+3);
        texture_index.push_back(6*i+4);
        texture_index.push_back(6*i+5);
    }
}

void CylinderRender::calculateModelPoint(float *RVertices, float *YVertices, int RVertexLen) {
    //top
    CylinderVertex.clear();
    CylinderFaces.clear();
    normals.clear();
    vertex_textures.clear();
    texture_index.clear();
    circleVertexLen=0;

    int num_samples = 50;
    float texX = 0, texY = 1;
    float texY_step = -1.0f/(RVertexLen-1);
    for(int k = 0; k < RVertexLen - 1; k++) {
        for (int i = 0; i <num_samples; i++) {
            CylinderVertex.push_back(RVertices[k] * cos(i * PI / (num_samples / 2)));
            CylinderVertex.push_back(YVertices[k]);
            CylinderVertex.push_back(RVertices[k] * sin(i * PI / (num_samples / 2)));

//            //用圆柱法向量
//            normals.push_back(RVertices[k] * cos(i * PI / (num_samples / 2)));
//            normals.push_back(0);
//            normals.push_back(RVertices[k] * sin(i * PI / (num_samples / 2)));

            //正确的法向量

            glm::vec3 prev = (k > 0 ? glm::vec3(RVertices[k-1] * cos(i * PI / (num_samples / 2)),YVertices[k-1], RVertices[k-1] * sin(i * PI / (num_samples / 2))) : glm::vec3(RVertices[k] * cos(i * PI / (num_samples / 2)),YVertices[k], RVertices[k] * sin(i * PI / (num_samples / 2))));
            glm::vec3 next = glm::vec3(RVertices[k+1] * cos(i * PI / (num_samples / 2)), YVertices[k+1], RVertices[k+1] * sin(i * PI / (num_samples / 2)));
            glm::vec3 lineT = (next - prev);
            glm::vec3 rotT = glm::vec3(RVertices[k] * sin(i * PI / (num_samples / 2)), 0, -RVertices[k] * cos(i * PI / (num_samples / 2)));
            glm::vec3 normal = glm::cross(lineT, rotT);
            glm::normalize(normal);
            normals.push_back(normal.x);
            normals.push_back(normal.y);
            normals.push_back(normal.z);

            // 面片1
            CylinderFaces.push_back(k * num_samples + i);
            CylinderFaces.push_back(k * num_samples + (i) + num_samples);
            CylinderFaces.push_back(k * num_samples + (i + 1) % num_samples);

            // 面片2
            CylinderFaces.push_back(k * num_samples + (i + 1) % num_samples);
            CylinderFaces.push_back(k * num_samples + (i) + num_samples);
            CylinderFaces.push_back(k * num_samples + (i + 1) % (num_samples) + num_samples);

            vertex_textures.push_back(1.0 * i / num_samples);
            vertex_textures.push_back(texY + k * texY_step);
        }
    }

    for (int i = 0; i <num_samples; i++) {
        CylinderVertex.push_back(RVertices[RVertexLen-1] * cos(i * PI / (num_samples / 2)));
        CylinderVertex.push_back(YVertices[RVertexLen-1]);
        CylinderVertex.push_back(RVertices[RVertexLen-1] * sin(i * PI / (num_samples / 2)));

//        //用圆柱法向量
//        normals.push_back(RVertices[RVertexLen-1] * cos(i * PI / (num_samples / 2)));
//        normals.push_back(0);
//        normals.push_back(RVertices[RVertexLen-1] * sin(i * PI / (num_samples / 2)));

        glm::vec3 prev = glm::vec3(RVertices[RVertexLen-2] * cos(i * PI / (num_samples / 2)),YVertices[RVertexLen-2], RVertices[RVertexLen-2] * sin(i * PI / (num_samples / 2)));
        glm::vec3 next = glm::vec3(RVertices[RVertexLen-1] * cos(i * PI / (num_samples / 2)), YVertices[RVertexLen-1], RVertices[RVertexLen-1] * sin(i * PI / (num_samples / 2)));
        glm::vec3 lineT = (next - prev);
        glm::vec3 rotT = glm::vec3(RVertices[RVertexLen-1] * sin(i * PI / (num_samples / 2)), 0, RVertices[RVertexLen-1] * cos(i * PI / (num_samples / 2)));
        glm::vec3 normal = glm::cross(lineT, rotT);
        glm::normalize(normal);
        normals.push_back(normal.x);
        normals.push_back(normal.y);
        normals.push_back(normal.z);

        vertex_textures.push_back(1.0 * i / num_samples);
        vertex_textures.push_back(texY + (RVertexLen-1) * texY_step);
    }
}

//void CylinderRender::calculateModelPoint(float *RVertices, float *YVertices, int RVertexLen) {
//    //top
//    CylinderVertex.clear();
//    CylinderFaces.clear();
//    normals.clear();
//    vertex_textures.clear();
//    texture_index.clear();
//    circleVertexLen=0;
//
//    int num_samples = 50;
//    for(int k = 0; k < RVertexLen; k++) {
//        for (int i = 0; i <num_samples; i++) {
//            CylinderVertex.push_back(RVertices[k] * cos(i * PI / (num_samples / 2)));
//            CylinderVertex.push_back(YVertices[k]);
////            CylinderVertex.push_back(YVertices[RVertexLen-k-1]);
//            CylinderVertex.push_back(RVertices[k] * sin(i * PI / (num_samples / 2)));
//
//            //用圆柱法向量
//            normals.push_back(RVertices[k] * cos(i * PI / (num_samples / 2)));
//            normals.push_back(0);
//            normals.push_back(RVertices[k] * sin(i * PI / (num_samples / 2)));
//        }
////        colors.push_back(top_r * cos(i * PI / (num_samples/2)));
////        colors.push_back(0);
////        colors.push_back(top_r * sin(i * PI / (num_samples/2)));
//        for (int i = 0; i <num_samples; i++) {
//            //bottom
//            CylinderVertex.push_back(RVertices[k + 1] * cos(i * PI / (num_samples / 2)));
//            CylinderVertex.push_back(YVertices[k + 1]);
////            CylinderVertex.push_back(YVertices[RVertexLen-k-2]);
//            CylinderVertex.push_back(RVertices[k + 1] * sin(i * PI / (num_samples / 2)));
//
//            normals.push_back(RVertices[k + 1] * cos(i * PI / (num_samples / 2)));
//            normals.push_back(0);
//            normals.push_back(RVertices[k + 1] * sin(i * PI / (num_samples / 2)));
//        }
////        colors.push_back(top_r * cos(i * PI / (num_samples/2)));
////        colors.push_back(0);
////        colors.push_back(top_r * sin(i * PI / (num_samples/2)));
//        for (int i = 0; i <num_samples; i++) {
//            // 面片1
//            CylinderFaces.push_back(k * num_samples + i);
//            CylinderFaces.push_back(k * num_samples + (i + 1) % num_samples);
//            CylinderFaces.push_back(k * num_samples + (i) + num_samples);
//
//
//            // 面片2
//            CylinderFaces.push_back(k * num_samples + (i) + num_samples);
//            CylinderFaces.push_back(k * num_samples + (i + 1) % num_samples);
//            CylinderFaces.push_back(k * num_samples + (i + num_samples + 1) % (num_samples) + num_samples);
//
//            // 面片1对应的顶点的纹理坐标
//            vertex_textures.push_back(1.0 * i / num_samples);
////            vertex_textures.push_back(1-k/(RVertexLen/2-1));
//            vertex_textures.push_back(0.0f);
//            vertex_textures.push_back(1.0 * (i + 1) / num_samples);
////            vertex_textures.push_back(1-k/( RVertexLen/2-1));
//            vertex_textures.push_back(0.0f);
//            vertex_textures.push_back(1.0 * i / num_samples);
////            vertex_textures.push_back(1-(k+1)/ (RVertexLen/2-1));
//            vertex_textures.push_back(1.0f);
//            // 对应的三角面片的纹理坐标的下标
//            texture_index.push_back(6 * i);
//            texture_index.push_back(6 * i + 1);
//            texture_index.push_back(6 * i + 2);
//
//            // 面片2对应的顶点的纹理坐标
//            vertex_textures.push_back(1.0 * i / num_samples);
////            vertex_textures.push_back(1-(k+1)/( RVertexLen/2-1));
//            vertex_textures.push_back(1.0f);
//            vertex_textures.push_back(1.0 * (i + 1) / num_samples);
////            vertex_textures.push_back(1-k/(RVertexLen/2-1));
//            vertex_textures.push_back(0.0f);
//            vertex_textures.push_back(1.0 * (i + 1) / num_samples);
////            vertex_textures.push_back(1-(k+1)/(RVertexLen/2-1));
//            vertex_textures.push_back(1.0f);
//            // 对应的三角面片的纹理坐标的下标
//            texture_index.push_back(6 * i + 3);
//            texture_index.push_back(6 * i + 4);
//            texture_index.push_back(6 * i + 5);
//        }
//    }
//}

void CylinderRender::setShader(Shader _shader) {
    shader = _shader;
}

void CylinderRender::Draw(string filename, const Camera &camera, const glm::vec3 position, const glm::vec3 size,
                          const float rotate, const glm::vec3 axis, glm::vec3 lightPos, glm::vec3 specular, float shininess, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 lightSpecular) {

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (CylinderVertex.size() + normals.size() + vertex_textures.size()), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * CylinderVertex.size(), CylinderVertex.data());
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * CylinderVertex.size(), sizeof(float) * normals.size(), normals.data());
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * (CylinderVertex.size()+normals.size()), sizeof(float) * vertex_textures.size(), vertex_textures.data());

    setTexture(filename);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * CylinderFaces.size() , CylinderFaces.data(), GL_STATIC_DRAW);

    // 顶点位置属性
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    // 顶点法线属性
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * CylinderVertex.size()));
    // 顶点纹理坐标
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * (CylinderVertex.size()+normals.size())));


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, texture);

    // 使用着色器程序
    shader.use();
    shader.use();
    shader.setInt("material.diffuse", 0);
    shader.setVec3("light.position", lightPos);
    shader.setVec3("viewPos", camera.Position);


    // light properties
    shader.setVec3("light.ambient", ambient);
    shader.setVec3("light.diffuse", diffuse);
    shader.setVec3("light.specular", lightSpecular);

    if(lightSet == 0) {
        // material properties
        shader.setVec3("material.specular", specular);
        shader.setFloat("material.shininess", shininess);
    }

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
    glDrawElements(GL_TRIANGLES, CylinderFaces.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void CylinderRender::Draw(int fileId, const Camera &camera, const glm::vec3 position, const glm::vec3 size,
                          const float rotate, const glm::vec3 axis, glm::vec3 lightPos, glm::vec3 specular, float shininess, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 lightSpecular) {

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (CylinderVertex.size() + normals.size() + vertex_textures.size()), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * CylinderVertex.size(), CylinderVertex.data());
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * CylinderVertex.size(), sizeof(float) * normals.size(), normals.data());
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * (CylinderVertex.size()+normals.size()), sizeof(float) * vertex_textures.size(), vertex_textures.data());

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * CylinderFaces.size() , CylinderFaces.data(), GL_STATIC_DRAW);

    // 顶点位置属性
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    // 顶点法线属性
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * CylinderVertex.size()));
    // 顶点纹理坐标
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * (CylinderVertex.size()+normals.size())));


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, textureIdVector[fileId]);

    // 使用着色器程序
    shader.use();
    shader.use();
    shader.setInt("material.diffuse", 0);
    shader.setVec3("light.position", lightPos);
    shader.setVec3("viewPos", camera.Position);


    // light properties
    shader.setVec3("light.ambient", ambient);
    shader.setVec3("light.diffuse", diffuse);
    shader.setVec3("light.specular", lightSpecular);

    if(lightSet == 0) {
        // material properties
        shader.setVec3("material.specular", specular);
        shader.setFloat("material.shininess", shininess);
    }

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
    glDrawElements(GL_TRIANGLES, CylinderFaces.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void CylinderRender::Draw(float r_top, float r_bot, float height, const Camera &camera, const glm::vec3 position, const glm::vec3 size,
                          const float rotate, const glm::vec3 axis) {
    calculateCirclePoint(r_top,r_bot,height+0.005f,height);

    setTexture("../resources/textures/container.jpg");

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(float) * circleVertexLen + vertex_textures.size(), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * circleVertexLen, circleVertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * circleVertexLen, sizeof(float) * circleVertexLen, normals.data());
    glBufferSubData(GL_ARRAY_BUFFER, 2 * sizeof(float) * circleVertexLen, vertex_textures.size(), vertex_textures.data());

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * faces.size() , faces.data(), GL_STATIC_DRAW);


    // 顶点位置属性
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    // 顶点法线属性
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(circleVertexLen * sizeof(float)));
    // 顶点纹理坐标
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(2 * circleVertexLen * sizeof(float)));


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, texture);

    glm::vec3 lightPos(0.0f, 5.0f, -7.0f);

    // 使用着色器程序
    shader.use();
    shader.use();
    shader.setInt("material.diffuse", 0);
    shader.setVec3("light.position", lightPos);
    shader.setVec3("viewPos", camera.Position);

    // light properties
    shader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    shader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

    // material properties
    shader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
    shader.setFloat("material.shininess", 64.0f);


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
    glDrawElements(GL_TRIANGLES, faces.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

void CylinderRender::setTexture(string filename) {
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, hei, nrChannels;
    data = stbi_load(filename.c_str(), &width, &hei, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, hei, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

int CylinderRender::setTexture(int index) {
    unsigned int tex;
    glGenTextures(1, &tex);

    glBindTexture(GL_TEXTURE_2D, tex); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, hei, nrChannels;
    unsigned char* data = stbi_load(textureFilenameVector[index].c_str(), &width, &hei, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, hei, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    return tex;
}

void CylinderRender::setDiffuseTexture(string filename) {
    glBindTexture(GL_TEXTURE_2D, diffuseMap); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, hei, nrChannels;
    data = stbi_load(filename.c_str(), &width, &hei, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, hei, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

void CylinderRender::setSpecularTexture(string filename) {
    glBindTexture(GL_TEXTURE_2D, specularMap); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, hei, nrChannels;
    data = stbi_load(filename.c_str(), &width, &hei, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, hei, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}