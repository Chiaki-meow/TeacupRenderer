#include "shader.h"
#include "camera.h"
#include "model.h"
#include "stb_image.h"
#include "settings.h"
#include "Game.h"
#include "BasicGUI.h"
#include "BezierLine.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"
#include "CircleRender.h"
#include "CylinderRender.h"
#include "./ParticleSystem/ParticleSystem.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void updateCheck(BasicGUI &basicGui);

// camera
Camera camera(glm::vec3(0.0f, 0.3f, -3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

bool drawLine = false;
bool drawEnd = false;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(-0.2f, -1.0f, -0.3f);

BasicGUI basicGui;

Game game;
SpriteRenderer  *Renderer;
Particle2D::ParticleSystemManager* particleSystemManager;

BezierLine bezierLine;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(SCR_WIDTH),
                                      static_cast<GLfloat>(SCR_HEIGHT), 0.0f, -1.0f, 1.0f);

//    game.setGame(SCR_WIDTH, SCR_HEIGHT, window, camera);

//    CircleRender circleRender;
//    CylinderRender cylinderRender;
//    BasicGUI basicGui(window);
    basicGui.setWindow(window);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);

    // build and compile shaders
    // -------------------------
    Shader skyboxShader("../shaders/skybox/vs.shader", "../shaders/skybox/fs.shader");
    Shader lineShader("../shaders/line/vs.shader", "../shaders/line/fs.shader");
    Shader bezierShader("../shaders/line/vs.shader", "../shaders/line/fs.shader");
    Shader spriteShader("../shaders/sprite/vs.shader", "../shaders/sprite/fs.shader");
    Shader circleShader("../shaders/circle/vs.shader", "../shaders/circle/fs.shader");
    Shader cylinderShader("../shaders/cylinder/vs.shader", "../shaders/cylinder/fs.shader");
    Shader particleShader("../shaders/particle/vs.shader", "../shaders/particle/fs.shader");

    bezierLine.setShader(lineShader, bezierShader);

    SpriteRenderer spriteRender(spriteShader);

    particleSystemManager = new Particle2D::ParticleSystemManager;
    Particle2D::ParticleSystem* PS = new Particle2D::ParticleSystem("Petal", particleShader, 3);
    PS->initWithPlist("../resources/configs/petal.plist");
    PS->getEmitter()->setEmiterPosition(glm::vec2(SCR_WIDTH, SCR_HEIGHT) * 0.5f);
    PS->getEmitter()->getParticleEffect()->motionMode = Particle2D::MotionMode::MOTION_MODE_RELATIVE;
    particleSystemManager->appendParticleSystem(PS);

    // load textures
    // -------------
//    unsigned int cubeTexture = loadTexture("../resources/textures/container.jpg");
    Texture2D map = ResourceManager::loadTextureFromFile("../resources/textures/container.jpg", GL_TRUE);

    // shader configuration
    // --------------------

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    spriteShader.use();
    spriteShader.setMat4("projection", projection);
    spriteShader.setInt("image", 0);
    spriteShader.setVec3("uniColor", glm::vec3(0,1,1));

    particleShader.use();
    particleShader.setInt("sprite", 0);
    particleShader.setMat4("projection", projection);

    SkyboxRender skyboxRender(skyboxShader);

    basicGui.bezierVector.push_back(&bezierLine);
    basicGui.objectName.push_back("Object 0");

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);
//        basicGui.bezierVector[basicGui.nowBezierLine] = &bezierLine;

        particleSystemManager->update(deltaTime);
        updateCheck(basicGui);

        // render
        // ------
        basicGui.Draw();
        ImGui::Render();

        if(basicGui.skyboxChange) {
            if(skyboxRender.faceID!=basicGui.skyboxID) {
                skyboxRender.faceID = basicGui.skyboxID;
                skyboxRender.faceChange = true;
            }
            basicGui.skyboxChange = false;
        }

        glm::mat4 projection3D = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 25.0f);
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        skyboxRender.Draw(camera, projection3D);
        spriteRender.Draw3DSprite("../resources/textures/container.jpg", camera, glm::vec3(-500.0f,-1000.0f,-500.0f), glm::vec3(1000.0f), -55.0f, glm::vec3(1.0f, 0.0f, 0.0f));

        for(int i=0;i<basicGui.bezierVector.size();i++){
            if(basicGui.nowBezierLine == i)
                continue;
            basicGui.bezierVector[i]->RenderModel(camera, 0.0f, glm::vec3(1.0f));
        }

        basicGui.bezierVector[basicGui.nowBezierLine]->RenderModel(basicGui.faceID, camera, glm::vec3(basicGui.position[0],basicGui.position[1],basicGui.position[2]), glm::vec3(basicGui.size[0]), 0.0f, glm::vec3(1.0f), glm::vec3(basicGui.lightPos[0],basicGui.lightPos[1],basicGui.lightPos[2]), glm::vec3(basicGui.specular[0],basicGui.specular[1],basicGui.specular[2]), basicGui.shininess[0], glm::vec3(basicGui.ambient[0],basicGui.ambient[1],basicGui.ambient[2]), glm::vec3(basicGui.diffuse[0],basicGui.diffuse[1],basicGui.diffuse[2]), glm::vec3(basicGui.lightSpecular[0],basicGui.lightSpecular[1],basicGui.lightSpecular[2]));
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if(basicGui.drawFlag)
            basicGui.bezierVector[basicGui.nowBezierLine]->Draw(glm::vec2 (0.0f), projection);
        particleSystemManager->render();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS){
        //用于继续渲染
        if(drawLine){
            drawLine = false;
            basicGui.bezierVector[basicGui.nowBezierLine]->CalculateR();
            basicGui.bezierVector[basicGui.nowBezierLine]->HelpCalculate();
//            bezierLine.CalculateR();
//            bezierLine.HelpCalculate();
        }
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        drawEnd = true;
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        basicGui.bezierVector[basicGui.nowBezierLine]->Clear();
        basicGui.bezierVector[basicGui.nowBezierLine]->HelpCalculate();
//        bezierLine.Clear();
//        bezierLine.HelpCalculate();
    }
}

void updateCheck(BasicGUI &basicGui){
    if(basicGui.ifSave){
        basicGui.bezierVector[basicGui.nowBezierLine]->saveModel(basicGui.filename);
        basicGui.ifSave = false;
    }
    if(basicGui.ifLoad){
        basicGui.bezierVector[basicGui.nowBezierLine]->loadModel(basicGui.filename);
        basicGui.ifLoad = false;
        basicGui.drawFlag = true;
        drawLine = true;
        drawEnd = false;
        return;
    }
    if(drawEnd){
        basicGui.drawFlag = false;
        drawLine = false;
        drawEnd = false;
        return;
    }
    if(basicGui.drawFlag) {
        drawLine = true;
        drawEnd = false;
        return;
    }
//    if(!basicGui.drawFlag && !drawEnd) {
//        drawLine = false;
//        bezierLine.Clear();
//        bezierLine.HelpCalculate();
//        return;
//    }
    //一直修改参数
//    basicGui.bezierVector[basicGui.nowBezierLine] = &bezierLine;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mod){
    if(drawLine){
        if(action == GLFW_PRESS){
            switch (button){
                case GLFW_MOUSE_BUTTON_LEFT:{
                    basicGui.bezierVector[basicGui.nowBezierLine]->AddNewPoint(lastX, lastY);
                    basicGui.bezierVector[basicGui.nowBezierLine]->DebugCheck();
//                    bezierLine.AddNewPoint(lastX, lastY);
//                    bezierLine.DebugCheck();
                    break;
                }
                case GLFW_MOUSE_BUTTON_RIGHT:{
                    basicGui.bezierVector[basicGui.nowBezierLine]->DeleteAddPoint();
//                    bezierLine.DeleteAddPoint();
                    break;
                }
                default:
                    break;
            }
        }
    }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}