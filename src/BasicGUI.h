//
// Created by 程佳雯 on 2021/12/31.
//

#ifndef GLFWSAMPLE_BASICGUI_H
#define GLFWSAMPLE_BASICGUI_H

#include "settings.h"
#include "BezierLine.h"

class BasicGUI {
public:
    BasicGUI(GLFWwindow* window);
    BasicGUI(){}
    void setWindow(GLFWwindow* window);
    ~BasicGUI();
    void Draw();

    //bool参数，用于确定现在到哪了
    bool drawFlag = false;
    int faceID = 0;
    bool ifSave = false;
    bool ifLoad = false;
    bool skyboxChange = false;
    int skyboxID = 0;

    float lightPos[3] = {0.0f, 5.0f, -7.0f};

    char filename[128] = "data.txt";

    float ambient[3] = {0.5f, 0.5f, 0.5f};
    float diffuse[3] = {0.5f, 0.5f, 0.5f};
    float lightSpecular[3] = {1.0f, 1.0f, 1.0f};

    float position[3] = {0.0f,0.05f,-4.0f};
    float size[1] = {0.1f};

    float specular[3] = {0.5f, 0.5f, 0.5f};
    float shininess[1] = {64.0f};

    int nowBezierLine = 0;

    vector<string> objectName;

    vector<BezierLine *> bezierVector;


//        // light properties
//    shader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
//    shader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
//    shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);


private:
    GLFWwindow* window;
    void initMainGui();
    bool show_demo_window;
    bool show_another_window;
    bool show_scene_window;
    ImVec4 clear_color;
};


#endif //GLFWSAMPLE_BASICGUI_H
