//
// Created by 程佳雯 on 2021/12/31.
//


#include "BasicGUI.h"

BasicGUI::BasicGUI(GLFWwindow* window) {
    this->window = window;
    skyboxChange = false;
    initMainGui();
}

void BasicGUI::setWindow(GLFWwindow *window) {
    this->window = window;
    skyboxChange = false;
    initMainGui();
}

BasicGUI::~BasicGUI() {

}

void BasicGUI::initMainGui() {
    const char* glsl_version = "#version 150";
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
    show_demo_window = true;
    show_another_window = false;
    show_scene_window = false;
    clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
}

void BasicGUI::Draw() {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
//        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Model Edit Window", &show_another_window);
        ImGui::Checkbox("Model Scene Window", &show_scene_window);

//        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
//        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Skybox Pool")) {
            skyboxID = 0;
            skyboxChange = true;
            cout<<"skybox Change"<<endl;
        }
        ImGui::SameLine();
        if (ImGui::Button("Skybox Sunset")) {
            skyboxID = 1;
            skyboxChange = true;
            cout<<"skybox Change"<<endl;

        }
        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window)
    {
        ImGui::Begin("MainWindow", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Draw your own line");
        ImGui::Text("Please decide your texture before rendering.");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        if(ImGui::Button("start Draw")) {
            if(drawFlag)
            drawFlag = false;
            else drawFlag = true;
        }

        if(drawFlag){
            ImGui::GetForegroundDrawList()->AddLine(ImVec2(SCR_WIDTH/2, 0), ImVec2(SCR_WIDTH/2, SCR_HEIGHT), ImGui::GetColorU32(ImGuiCol_Button), 4.0f); // Draw a line between the button and the mouse cursor
        }
//        if(ImGui::Button("Save")) {
//            ifSave = true;
//        }
//        ImGui::SameLine();
//        if(ImGui::Button("Load")) {
//            ifLoad = true;
//        }
        ImGui::InputText("filename", filename,  IM_ARRAYSIZE(filename));

        if(ImGui::Button("Save")) {
            if(filename!= nullptr) {
                ifSave = true;
            }
        }
        ImGui::SameLine();
        if(ImGui::Button("Load")) {
            if(filename!= nullptr) {
                ifLoad = true;
            }
        }
        ImGui::Text("Texture");
        if (ImGui::Button("wood1"))
            faceID = 0;
        ImGui::SameLine();
        if (ImGui::Button("wood2"))
            faceID = 1;
        if (ImGui::Button("stone1"))
            faceID = 2;
        ImGui::SameLine();
        if (ImGui::Button("stone2"))
            faceID = 3;
        if (ImGui::Button("china1"))
            faceID = 4;
        ImGui::SameLine();
        if (ImGui::Button("china2"))
            faceID = 5;

//        ImGui::SliderFloat2("lightPos", lightPos, -100.0, 100.0);
        ImGui::InputFloat3("position", position);
        ImGui::InputFloat3("lightPos", lightPos);
        ImGui::Text("light properties");
        ImGui::InputFloat3("ambient", ambient);
        ImGui::InputFloat3("diffuse", diffuse);
        ImGui::InputFloat3("lightSpecular", lightSpecular);
        ImGui::Text("material");
        ImGui::InputFloat3("specular", specular);
        ImGui::InputFloat("shininess", shininess);
        ImGui::End();
    }
    if(show_scene_window){
        ImGui::Begin("ModelWindow", &show_scene_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::InputFloat3("position", position);
        ImGui::InputFloat("size", size);
        if(ImGui::Button("add object")){
            bezierVector.push_back(new BezierLine());
            nowBezierLine = bezierVector.size()-1;
            Shader lineShader("../shaders/line/vs.shader", "../shaders/line/fs.shader");
            Shader bezierShader("../shaders/line/vs.shader", "../shaders/line/fs.shader");
            bezierVector[nowBezierLine]->setShader(lineShader, bezierShader);
            objectName.push_back("Object " + to_string(nowBezierLine));
        }
        for (int n = 0; n < bezierVector.size(); n++)
        {
            const bool is_selected = (nowBezierLine == n);
            if (ImGui::Selectable(objectName[n].c_str(), is_selected))
                nowBezierLine = n;
            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::End();
    }
}