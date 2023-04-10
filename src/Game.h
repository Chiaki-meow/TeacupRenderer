//
// Created by 程佳雯 on 2021/12/28.
//

#ifndef GAME_H
#define GAME_H

#include "settings.h"

#include "ResourceManager.h"
#include "SkyboxRender.h"
#include "CubeTexture.h"
#include "CubeRender.h"
#include "BasicGUI.h"
#include "BezierLine.h"

enum GameState{
    Edit, Render
};

class Game
{
public:
    // 控制的行为参数

    bool drawLine = false;

    // Game state
    static GameState state;

    static shared_ptr<SkyboxRender> skyboxRender;
    static shared_ptr<CubeRender> cubeRender;

    GLboolean Keys[1024];
    GLuint Width, Height;

    Camera camera;
    GLFWwindow* window;

    BasicGUI basicGui;
    BezierLine bezierLine;


    //shader
    Shader skyboxShader;
    Shader lineShader;
    Shader bezierShader;

    // Constructor/Destructor
    Game();
    Game(GLuint width, GLuint height, GLFWwindow* window, Camera camera);
    void setGame(GLuint width, GLuint height, GLFWwindow* window, Camera camera);

    ~Game();
    // Initialize game state (load all shaders/textures/levels)
    void Init();
    // GameLoop
    void ProcessInput(GLfloat dt);
    void Update(GLfloat dt);
    void Render();

private:
    void LoadShader();
    void LoadTexture();
    void Start();
};

#endif //GLFWSAMPLE_GAME_H
