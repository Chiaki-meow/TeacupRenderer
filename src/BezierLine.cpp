//
// Created by 程佳雯 on 2021/12/31.
//

#include "BezierLine.h"

double combination(int n, int m)
{
    double sum = 1;
    for (int i = 1; i <= m; i++) {
        sum = sum * (n - m + i) / i;
    }
    return sum;
}

BezierLine::BezierLine(Shader _shader, Shader _bezierShader) : shader(_shader), bezierShader(_bezierShader){
    init();
}

BezierLine::~BezierLine() {
    delete cylinderRender;
}

void BezierLine::AddNewPoint(float xPos, float yPos) {
    // 每隔两个点画一条直线
    // 鼠标点击的点
    lineVertices[lineVertexLen] = xPos;
    lineVertexLen++;
    lineVertices[lineVertexLen] = yPos;
    lineVertexLen++;
    lineVertices[lineVertexLen] = 0.0f;
    lineVertexLen++;
    // 添加索引,前一个点也新的点一起确定新线段
    if (lineIndicesLen >= 2) {
        lineIndices[lineIndicesLen] = lineIndices[lineIndicesLen - 1];
        lineIndicesLen++;
        lineIndices[lineIndicesLen] = lineIndices[lineIndicesLen - 1] + 1;
        lineIndicesLen++;
    }
    else {
        lineIndices[lineIndicesLen] = lineIndicesLen;
        lineIndicesLen++;
    }
}

void BezierLine::DeleteAddPoint() {
    if (lineVertexLen >= 3) {
        lineVertexLen = lineVertexLen - 3;
    }
    if(lineIndicesLen == 0)
        return;
    if (lineIndicesLen >= 4) {
        lineIndicesLen = lineIndicesLen - 2;
    }
    else {
        lineIndicesLen = lineIndicesLen - 1;
    }
}

int BezierLine::CalCurvePoint() {
    int bezierVertexLen = 0;
    float firstY = 0;
    RVertexLen = 0;
    if (lineVertexLen == 0) return bezierVertexLen;
    else if (lineVertexLen == 3) {
        bezierVertices[bezierVertexLen] = lineVertices[0];
        bezierVertexLen++;
        bezierVertices[bezierVertexLen] = lineVertices[1];
        bezierVertexLen++;
        bezierVertices[bezierVertexLen] = lineVertices[2];
        bezierVertexLen++;
    }
    else {
        for(float t = 0.000f; t < 1.000f; t = t + 0.005f){
            float new_xPos = 0, new_yPos = 0;
            for(int index = 0; index < lineVertexLen; index++){
                new_xPos += lineVertices[index * 3] *
                            pow(t, index) * pow(1 - t, lineVertexLen / 3 - index - 1) *
                            combination(lineVertexLen / 3 - 1, index);
                // y coordinate
                new_yPos += lineVertices[index * 3 + 1] *
                            pow(t, index) * pow(1 - t, lineVertexLen / 3 - index - 1) *
                            combination(lineVertexLen / 3 - 1, index);
            }
            bezierVertices[bezierVertexLen] = new_xPos;
//            RVertices[bezierVertexLen/3] = abs(bezierVertices[bezierVertexLen] - SCR_WIDTH/2)/(SCR_WIDTH/2); //计算r
            RVertices[bezierVertexLen/3] =(bezierVertices[bezierVertexLen] - SCR_WIDTH/2)<0 ? abs(bezierVertices[bezierVertexLen] - SCR_WIDTH/2)/(SCR_WIDTH/2) : 0; //计算r

            RVertexLen++;
            bezierVertexLen++;
            bezierVertices[bezierVertexLen] = new_yPos;
            YVertices[(bezierVertexLen-1)/3] = abs(bezierVertices[bezierVertexLen] - SCR_HEIGHT)/(SCR_HEIGHT);

            bezierVertexLen++;
            bezierVertices[bezierVertexLen] = 0.0f;
            bezierVertexLen++;
        }
    }
    return bezierVertexLen;
}

void BezierLine::init() {
    glGenVertexArrays(1, &bezierVAO);
    glGenBuffers(1, &bezierVBO);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    this->textureVector.emplace_back("../resources/textures/wood1.jpg");
    this->textureVector.emplace_back("../resources/textures/wood2.jpg");
    this->textureVector.emplace_back("../resources/textures/stone1.jpg");
    this->textureVector.emplace_back("../resources/textures/stone2.jpg");
    this->textureVector.emplace_back("../resources/textures/china1.jpg");
    this->textureVector.emplace_back("../resources/textures/china2.jpg");

    cylinderRender = new CylinderRender();
    Shader cylinderShader("../shaders/cylinder/vs.shader", "../shaders/cylinder/fs.shader");
    cylinderRender->setShader(cylinderShader);
//    startRender = false;
}

void BezierLine::Draw(){
    int lineVertexLen = 9;
    float lineVertices[300] = {
            //位置
            -0.5f, 0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.0f
    };
    int lineIndicesLen = 4;
    unsigned int lineIndices[4] = { // 注意索引从0开始!
            0, 1, // 第一个线段
            1, 2
    };
    bezierVertexLen = CalCurvePoint();
    // paint the straight lines
    shader.use();

    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(SCR_WIDTH),static_cast<GLfloat>(SCR_HEIGHT), 0.0f, -1.0f, 1.0f);
    shader.setMat4("projection",projection);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * lineVertexLen, lineVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * lineIndicesLen, lineIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    glBindVertexArray(VAO);
    glLineWidth(3);
    if (lineIndicesLen >= 2) {
        glDrawElements(GL_LINES, lineIndicesLen, GL_UNSIGNED_INT, 0);
    }

    bezierShader.use();
    bezierShader.setMat4("projection",projection);

    // paint the bezier curve
    glBindVertexArray(bezierVAO);
    glBindBuffer(GL_ARRAY_BUFFER, bezierVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * bezierVertexLen, bezierVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(bezierVAO);
    glPointSize(3);
    glDrawArrays(GL_POINTS, 0, bezierVertexLen / 3);
    glBindVertexArray(0);
}

void BezierLine::Draw(const glm::vec2& position, const glm::mat4 projection){
    bezierVertexLen = CalCurvePoint();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // paint the straight lines
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * lineVertexLen, lineVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * lineIndicesLen, lineIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // 使用着色器程序
    shader.use();
    shader.setMat4("projection", projection);
    glBindVertexArray(VAO);
    glLineWidth(3);
    if (lineIndicesLen >= 2) {
        glDrawElements(GL_LINES, lineIndicesLen, GL_UNSIGNED_INT, 0);
    }

    shader.use();
    shader.setMat4("projection",projection);

    glGenVertexArrays(1, &bezierVAO);
    glGenBuffers(1, &bezierVBO);
    // paint the bezier curve
    glBindVertexArray(bezierVAO);
    glBindBuffer(GL_ARRAY_BUFFER, bezierVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * bezierVertexLen, bezierVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindVertexArray(bezierVAO);
    glPointSize(3);
    glDrawArrays(GL_POINTS, 0, bezierVertexLen);
    glBindVertexArray(0);
}

void BezierLine::DebugCheck() {
    cout << lineIndicesLen << endl;
}

void BezierLine::Clear() {
    lineIndices[MAX_INDICES_LEN] = {};
    lineIndicesLen = 0;

    bezierVertices[MAX_BEZIER_VERTEX_LEN] = {};
    bezierVertexLen = 0;

    lineVertices[MAX_VERTEX_LEN] = {};
    lineVertexLen = 0;
}

bool BezierLine::CalculateR(float* & rVertices, float &height) {
    height = abs(bezierVertices[bezierVertexLen-2] - bezierVertices[1]);
    RVertexHeight = height;
    for (int i = 0; i < bezierVertexLen; i += 3) {
        if(bezierVertices[0+i] > SCR_WIDTH/2){
            // 画不出来
            return false;
        }
        rVertices[i] = (bezierVertices[i] - SCR_WIDTH/2)/(SCR_WIDTH/2); //计算r
    }
    return true;
}

bool BezierLine::CalculateR() {
    RVertexHeight = (bezierVertices[bezierVertexLen-2] - bezierVertices[1]);
    for (int i = 0; i < bezierVertexLen - 3; i += 3) {
        if(bezierVertices[0+i] > SCR_WIDTH/2){
            // 画不出来
            return false;
        }
    }
//    startRender = true;
    return true;
}

void BezierLine::RenderModel(int textureNameIndex, const Camera &camera, const glm::vec3 position, const glm::vec3 size,
                             const float rotate, const glm::vec3 axis, glm::vec3 lightPos, glm::vec3 specular, float shininess, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 lightSpecular) {
//    if(!startRender)
//        return;
    if(!changeData)
        return;
    this->facesID = textureNameIndex;
    this->position = position;
    this->size = size;
    this->lightPos = lightPos;
    this->specular = specular;
    this->shininess = shininess;
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->lightSpecular = lightSpecular;
    cylinderRender->Draw(textureNameIndex, camera, position, size, rotate, axis, lightPos, specular, shininess, ambient, diffuse, lightSpecular);
}

void BezierLine::RenderModel(const Camera &camera,const float rotate, const glm::vec3 axis) {
//    if(!startRender)
//        return;
    if(!changeData)
        return;
    cylinderRender->Draw(facesID, camera, position, size, rotate, axis, lightPos, specular, shininess, ambient, diffuse, lightSpecular);
}

void BezierLine::HelpCalculate() {
    changeData = true;
    bezierVertexLen = CalCurvePoint();
    cylinderRender->calculateModelPoint(RVertices, YVertices, RVertexLen);
}

void BezierLine::saveModel() {
    fstream f;
    f.open("data.txt",ios::out);
    if (!f.is_open()){
        cout << "Error opening file";
        return;
    }
    //输入你想写入的内容
    f<<lineIndicesLen<<" ";
    for(int i=0;i<lineIndicesLen;i++){
        f<<lineIndices[i]<<" ";
    }
    f<<bezierVertexLen<<" ";
    for(int i=0;i<bezierVertexLen;i++){
        f<<bezierVertices[i]<<" ";
    }
    f<<lineVertexLen<<" ";
    for(int i=0;i<lineVertexLen;i++){
        f<<lineVertices[i]<<" ";
    }
    f<<endl;
    f.close();
}

void BezierLine::loadModel() {
    ifstream in("test.txt");
    string str;
    in.open("data.txt",ios::in);
    if (!in.is_open()){
        cout << "Error opening file";
        return;
    }
        in>>lineIndicesLen;
        for(int i=0;i<lineIndicesLen;i++){
            in >> lineIndices[i];
        }
        in>>bezierVertexLen;
        for(int i=0;i<bezierVertexLen;i++){
            in>>bezierVertices[i];
        }
        in>>lineVertexLen;
        for(int i=0;i<lineVertexLen;i++){
            in>>lineVertices[i];
    }
    HelpCalculate();
}

void BezierLine::saveModel(string filename) {
    fstream f;
    f.open(filename,ios::out);
    if (!f.is_open()){
        cout << "Error opening file";
        return;
    }
    //输入你想写入的内容
    f<<lineIndicesLen<<" ";
    for(int i=0;i<lineIndicesLen;i++){
        f<<lineIndices[i]<<" ";
    }
    f<<bezierVertexLen<<" ";
    for(int i=0;i<bezierVertexLen;i++){
        f<<bezierVertices[i]<<" ";
    }
    f<<lineVertexLen<<" ";
    for(int i=0;i<lineVertexLen;i++){
        f<<lineVertices[i]<<" ";
    }
    f<<facesID<<" ";
    f<<shininess<<" ";
    f<<position.x<<" "<<position.y<<" "<<position.z<<" ";
    f<<size.x<<" "<<size.y<<" "<<size.z<<" ";
    f<<lightPos.x<<" "<<lightPos.y<<" "<<lightPos.z<<" ";
    f<<specular.x<<" "<<specular.y<<" "<<specular.z<<" ";
    f<<ambient.x<<" "<<ambient.y<<" "<<ambient.z<<" ";
    f<<diffuse.x<<" "<<diffuse.y<<" "<<diffuse.z<<" ";
    f<<lightSpecular.x<<" "<<lightSpecular.y<<" "<<lightSpecular.z<<" ";

    f<<endl;
    f.close();
}

void BezierLine::loadModel(string filename) {
    ifstream in("test.txt");
    string str;
    in.open(filename, ios::in);
    if (!in.is_open()){
        cout << "Error opening file";
        return;
    }
        in>>lineIndicesLen;
        for(int i=0;i<lineIndicesLen;i++){
            in >> lineIndices[i];
        }
        in>>bezierVertexLen;
        for(int i=0;i<bezierVertexLen;i++){
            in>>bezierVertices[i];
        }
        in>>lineVertexLen;
        for(int i=0;i<lineVertexLen;i++){
            in>>lineVertices[i];
    }
    in>>facesID;
    in>>shininess;
    in>>position.x>>position.y>>position.z;
    in>>size.x>>size.y>>size.z;
    in>>lightPos.x>>lightPos.y>>lightPos.z;
    in>>specular.x>>specular.y>>specular.z;
    in>>ambient.x>>ambient.y>>ambient.z;
    in>>diffuse.x>>diffuse.y>>diffuse.z;
    in>>lightSpecular.x>>lightSpecular.y>>lightSpecular.z;

    HelpCalculate();
}

void BezierLine::setTexture(int) {

}