//
// Created by 程佳雯 on 2021/12/28.
//

#include "Game.h"


shared_ptr<CubeRender> Game::cubeRender;
shared_ptr<SkyboxRender> Game::skyboxRender;
//Particle2D::ParticleSystemManager* particleSystemManager;

Game::Game(GLuint width, GLuint height, GLFWwindow* window, Camera camera) : Width(width), Height(height), window(window),
                                                                             camera(camera){
    Init();
}

Game::Game(){}

void Game::setGame(GLuint width, GLuint height, GLFWwindow* window, Camera camera){
    this->Width = width;
    this->Height = height;
    this->window = window;
    this->camera = camera;
    Init();
}


Game::~Game()
{

}

void Game::Init()
{
//    LoadShader();
//    LoadTexture();
//    Shader tmp = ResourceManager::GetShader("skybox");
//    // Set the renderer for skybox
//    skyboxRender = make_shared<SkyboxRender>(tmp);
//    // Set the renderer for cube
//    tmp = ResourceManager::GetShader("cube");
//    cubeRender = make_shared<CubeRender>(tmp);
    //我自己在用的
    basicGui.setWindow(window);

    // build and compile shaders
    // -------------------------
    Shader shader("../shaders/cubemap/vs.shader", "../shaders/cubemap/fs.shader");
//    skyboxShader = Shader skyboxShader("../shaders/skybox/vs.shader", "../shaders/skybox/fs.shader");

    Shader lineShader("../shaders/line/vs.shader", "../shaders/line/fs.shader");
    Shader bezierShader("../shaders/line/vs.shader", "../shaders/line/fs.shader");

    bezierLine.setShader(lineShader, bezierShader);

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    SkyboxRender skyboxRender(skyboxShader);

//    particleSystemManager = new Particle2D::ParticleSystemManager;
//    Particle2D::ParticleSystem* PS = new Particle2D::ParticleSystem("Petal", 3);
//    PS->initWithPlist("res/configs/petal.plist");
//    PS->getEmitter()->setEmiterPosition(glm::vec2(SCR_WIDTH, SCR_HEIGHT) * 0.5f);
//    PS->getEmitter()->getParticleEffect()->motionMode = Particle2D::MotionMode::MOTION_MODE_RELATIVE;
//    particleSystemManager->appendParticleSystem(PS);
}

void Game::Update(GLfloat dt)
{
    if(basicGui.drawFlag)
        this->drawLine = true;
    if(!basicGui.drawFlag)
        this->drawLine = false;
//    particleSystemManager->update(dt);
}


void Game::ProcessInput(GLfloat dt)
{
    static bool firstMouse = 1;
//    camera.ProcessInput(firstMouse, lastMouse, Window::Keys, dt);
}

void Game::Render()
{
    basicGui.Draw();
    ImGui::Render();
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(SCR_WIDTH),
                                      static_cast<GLfloat>(SCR_HEIGHT), 0.0f, -1.0f, 1.0f);
    glm::mat4 projection3D = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    skyboxRender->Draw(camera, projection3D);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    bezierLine.Draw(glm::vec2 (0.0f), projection);
//    particleSystemManager->render();
}

void Game::LoadShader(){
    //load shaders
    ResourceManager::LoadShader("../shaders/cubemap/vs.shader", "../shaders/cubemap/fs.shader", nullptr, "cube");
    ResourceManager::LoadShader("../shaders/skybox/vs.shader", "../shaders/skybox/fs.shader", nullptr, "skybox");
}

void Game::LoadTexture() {
    vector<std::string> faces {
        "../resources/textures/skybox/right.jpg",
        "../resources/textures/skybox/left.jpg",
        "../resources/textures/skybox/top.jpg",
        "../resources/textures/skybox/bottom.jpg",
        "../resources/textures/skybox/front.jpg",
        "../resources/textures/skybox/back.jpg",

    };
    ResourceManager::LoadCubeTexture(faces, GL_FALSE, "skybox");
}

void Game::Start(){

}