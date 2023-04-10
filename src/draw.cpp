#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>

#include "mathComputation.hpp"
#include "draw.h"

using namespace std;

int Sgn(int x){
    if (x>0)
        return 1;
    else if(x==0)
        return 0;
    else
        return -1;
}

drawMethod::drawMethod(Shader &shader){
    this->drawShader = shader;
    this->point = new pointManager(drawShader);
    for(int i=0;i<SCR_WIDTH;i++)
        for(int j=0;j<SCR_HEIGHT;j++)
            this->zBuffer[i][j]=-1000000;
}

float drawMethod::drawDepth(int x, int length, int mode){
    float retDepth;
    if(mode == 0){  //先下后上
        int dx = x%length;
        int nx = x/length;
        if(nx%2==0)
            retDepth = 1;
        else{
            retDepth = 20;
        }
    }

    if(mode == 1){  //先上后下
        int dx = x%length;
        int nx = x/length;
        if(nx%2==0)
            retDepth = 20;
        else
            retDepth = 1;
    }
    if(length == 0.2*80){
    if(mode == 0){  //先下后上
        int dx = (16+x)%32;
        int nx = (16+x)/32;
        if(nx%2==0)
            retDepth = 1;
        else{
            retDepth = 20;
        }
    }

    if(mode == 1){  //先上后下
        int dx = (16+x)%32;
        int nx = (16+x)/32;
        if(nx%2==0)
            retDepth = 20;
        else
            retDepth = 1;
    }
    }
    return retDepth;
}


void drawMethod::drawTriangle(vector<glm::vec3> positions, vector<glm::vec3> colors, int hw3){
    for (int i=0;i<SCR_HEIGHT;i++){
        //扫描线，扫描每一条横着的线 然后找交点画形状
        //保存扫描到的点的颜色和深度（用一些数学比例计算）
        Segment scanLine(Point(0,i),Point(SCR_WIDTH,i));
        vector<Point> crossPoints;
        vector<glm::vec3> bindColors;
        vector<float> bindDepth;
        int pos_size = positions.size();
        for(int j=0;j<pos_size;j++){
            //需要图形都是凸多边形并且都是顺序输入的（可以按照这个顺序进行连接）
            //判断每一条边和扫描线的交点
            Segment shapeLine(Point(positions[j].x,positions[j].y),Point(positions[(j+1)%pos_size].x,positions[(j+1)%pos_size].y));
            bool isInter = GeometryUtil::intersect(scanLine,shapeLine);
            if(isInter){
                Point crossPoint = GeometryUtil::getCrossPoint(scanLine,shapeLine);
                //扫描到了交点
                bool flag = true;
                //检查是否是已有的点
                for(auto& iter: crossPoints){
                    if(iter == crossPoint)
                        flag = false;
                        //先前已经扫描进去了，可能是两组线的相同交点，直接弃用
                }
                //确定有这个点
                if(flag){
                    crossPoints.push_back(crossPoint);
                    //矩阵运算，color,depth和坐标有关系
                    bindColors.push_back(float((crossPoint.y - positions[j].y) /(positions[(j + 1) % pos_size].y - positions[j].y)) * colors[(j + 1) % pos_size] + float((positions[(j + 1) % pos_size].y - crossPoint.y) /(positions[(j + 1) % pos_size].y - positions[j].y)) * colors[j]);
                    bindDepth.push_back(float((crossPoint.y - positions[j].y) /(positions[(j + 1) % pos_size].y - positions[j].y)) * positions[(j + 1) % pos_size].z + float((positions[(j + 1) % pos_size].y - crossPoint.y) /(positions[(j + 1) % pos_size].y - positions[j].y))* positions[j].z);
                }

            }
        }
        if(crossPoints.size()==2){
            //是凸多边形的话 只能是0/2
            if(crossPoints[0].x>crossPoints[1].x){
                swap(crossPoints[0],crossPoints[1]);
                swap(bindColors[0],bindColors[1]);
                swap(bindDepth[0],bindDepth[1]);
            }
            //采用额外的深度进行计算
            //只算横着的，竖着的都保持用横着的绕它就可以
            if(hw3 != 0){
                for(int j=crossPoints[0].x;j<crossPoints[1].x;j++){
                    float tmpDepth = drawDepth(j-crossPoints[0].x,hw3,hw3_flag);
                    if(tmpDepth >= zBuffer[j][i]){
                        zBuffer[j][i] = tmpDepth;
                        glm::vec3 color = float((j - crossPoints[0].x)/(crossPoints[1].x-crossPoints[0].x)) * bindColors[1] + float((crossPoints[1].x - j) /(crossPoints[1].x-crossPoints[0].x)) * bindColors[0];
                        point->drawPoint(glm::vec2(j,i),color);
                   
                    }
                }
            }
            else{
                //和扫描线相交区间的各个像素
                for(int j=crossPoints[0].x;j<crossPoints[1].x;j++){
                    float tmpDepth = float((j - crossPoints[0].x)/(crossPoints[1].x-crossPoints[0].x)) * bindDepth[1] + float((crossPoints[1].x - j) /(crossPoints[1].x-crossPoints[0].x)) * bindDepth[0];
                    if(tmpDepth >= zBuffer[j][i]){
                        zBuffer[j][i] = tmpDepth;
                        glm::vec3 color = float((j - crossPoints[0].x)/(crossPoints[1].x-crossPoints[0].x)) * bindColors[1] + float((crossPoints[1].x - j) /(crossPoints[1].x-crossPoints[0].x)) * bindColors[0];
                        point->drawPoint(glm::vec2(j,i),color);
                    }
                }
            }
        }
    }
}

//改成xy就可以改变位置了
void drawMethod::drawHW1(float posx,float posy){
    //pos调整data到800*600
    int data[] = {
		20, 0, 0, 100, 0, 0, -20, 50, 20, 1, 0, 0, 0, 1, 0, 0, 0, 1,
		0, 20, 0, 0, 100, 0, -50, -20, 20, 1, 0, 0, 0, 1, 0, 0, 0, 1,
		-20, 0, 0, -100, 0, 0, 20, -50, 20, 1, 0, 0, 0, 1, 0, 0, 0, 1,
		0, -20, 0, 0, -100, 0, 50, 20, 20, 1, 0, 0, 0, 1, 0, 0, 0, 1
	};

    for(int i=0;i<4;i++){
        vector<glm::vec3> positions;
        vector<glm::vec3> colors;
        
        positions.push_back(glm::vec3(data[0+18*i]+posx,data[1+18*i]+posy,data[2+18*i]));
        positions.push_back(glm::vec3(data[3+18*i]+posx,data[4+18*i]+posy,data[5+18*i]));
        positions.push_back(glm::vec3(data[6+18*i]+posx,data[7+18*i]+posy,data[8+18*i]));

        colors.push_back(glm::vec3(data[9+18*i],data[10+18*i],data[11+18*i]));
        colors.push_back(glm::vec3(data[12+18*i],data[13+18*i],data[14+18*i]));
        colors.push_back(glm::vec3(data[15+18*i],data[16+18*i],data[17+18*i]));

        drawTriangle(positions,colors,0);
    }

    for(int i=0;i<SCR_WIDTH;i++)
        for(int j=0;j<SCR_HEIGHT;j++)
            this->zBuffer[i][j]=-1000000;
}

void drawMethod::drawHW2(float posx,float posy){
	float data[] = {
		0, 0, 0, 100, 0, 0, 100, 100, 20, 1, 0, 0, 0, 1, 0, 0, 0, 1,
		0, 0, 20, 100, 0, 20, 100, 100, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0
	};

	glm::mat4 projection = glm::perspective(glm::radians(60.0f), static_cast<GLfloat>(SCR_WIDTH) /static_cast<GLfloat>(SCR_HEIGHT), 0.1f, 100.0f);

    glm::mat4 view;

    view = glm::lookAt(glm::vec3(0.5f, 0.0f, 1.0f), 
           glm::vec3(1.0f, -0.1f, 0.0f), 
           glm::vec3(0.0f, 1.0f, 0.0f));

    for(int i=0;i<2;i++){
        vector<glm::vec3> positions;
        vector<glm::vec3> colors;
        positions.push_back(projection*view*glm::vec4(data[0+18*i]+posx,data[1+18*i]+posy,-data[2+18*i],1));
        positions.push_back(projection*view*glm::vec4(data[3+18*i]+posx,data[4+18*i]+posy,-data[5+18*i],1));
        positions.push_back(projection*view*glm::vec4(data[6+18*i]+posx,data[7+18*i]+posy,-data[8+18*i],1));

        colors.push_back(glm::vec3(data[9+18*i],data[10+18*i],data[11+18*i]));
        colors.push_back(glm::vec3(data[12+18*i],data[13+18*i],data[14+18*i]));
        colors.push_back(glm::vec3(data[15+18*i],data[16+18*i],data[17+18*i]));

        drawTriangle(positions,colors,0);
    }

    for(int i=0;i<SCR_WIDTH;i++)
        for(int j=0;j<SCR_HEIGHT;j++)
            this->zBuffer[i][j]=-1000000;
}
void drawMethod::drawHW3(float posx,float posy){
    glm::vec3 colorPurple(glm::vec3(112.0f/255.0f,48.0f/255.0f,160.0f/255.0f)),colorBlue(glm::vec3(68.0f/255.0f,114.0f/255.0f,196.0f/255.0f));

    //竖着的
    for(int i=0;i<4;i++){
        vector<glm::vec3> positions;
        vector<glm::vec3> colors;
        
        positions.push_back(glm::vec3(0.2*80+0.2*160*i+posx,0+posy,10));
        positions.push_back(glm::vec3(0.2*160+0.2*160*i+posx,0+posy,10));
        positions.push_back(glm::vec3(0.2*160+0.2*160*i+posx,0.2*720+posy,10));
        positions.push_back(glm::vec3(0.2*80+0.2*160*i+posx,0.2*720+posy,10));

        colors.push_back(colorPurple);
        colors.push_back(colorPurple);
        colors.push_back(colorPurple);
        colors.push_back(colorPurple);

        drawTriangle(positions,colors,0);
    }

        //横着的
    for(int i=0;i<4;i++){
        vector<glm::vec3> positions;
        vector<glm::vec3> colors;
        
        positions.push_back(glm::vec3(0+posx,0.2*80+0.2*160*i+posy,0));
        positions.push_back(glm::vec3(0+posx,0.2*160+0.2*160*i+posy,0));
        positions.push_back(glm::vec3(0.2*720+posx,0.2*160+0.2*160*i+posy,0));
        positions.push_back(glm::vec3(0.2*720+posx,0.2*80+0.2*160*i+posy,0));

        colors.push_back(colorBlue);
        colors.push_back(colorBlue);
        colors.push_back(colorBlue);
        colors.push_back(colorBlue);

        drawTriangle(positions,colors,0.2*160);
        hw3_flag = !hw3_flag;

    }

    for(int i=0;i<SCR_WIDTH;i++)
        for(int j=0;j<SCR_HEIGHT;j++)
            this->zBuffer[i][j]=-1000000;
}

void drawMethod::drawHW3Up(float posx,float posy){
    glm::vec3 colorPurple(glm::vec3(255.0f/255.0f,250.0f/255.0f,240.0f/255.0f)),colorBlue(glm::vec3(245.0f/255.0f,222.0f/255.0f,179.0f/255.0f));
    glm::vec3 colorPurple1(glm::vec3(253.0f/255.0f,245.0f/255.0f,230.0f/255.0f)),colorBlue1(glm::vec3(255.0f/255.0f,228.0f/255.0f,181.0f/255.0f));

    //竖着的
    for(int i=0;i<4;i++){
        vector<glm::vec3> positions;
        vector<glm::vec3> colors;
        
        positions.push_back(glm::vec3(0.2*80+0.2*160*i+posx,0+posy,10));
        positions.push_back(glm::vec3(0.2*160+0.2*160*i+posx,0+posy,10));
        positions.push_back(glm::vec3(0.2*160+0.2*160*i+posx,0.2*640+posy,10));
        positions.push_back(glm::vec3(0.2*80+0.2*160*i+posx,0.2*640+posy,10));

        colors.push_back(colorPurple);
        colors.push_back(colorPurple);
        colors.push_back(colorPurple);
        colors.push_back(colorPurple);

        drawTriangle(positions,colors,0);
    }
    //竖着的2号
    for(int i=0;i<4;i++){
        vector<glm::vec3> positions;
        vector<glm::vec3> colors;
        
        positions.push_back(glm::vec3(0.2*160*i+posx,0+posy,10));
        positions.push_back(glm::vec3(0.2*80+0.2*160*i+posx,0+posy,10));
        positions.push_back(glm::vec3(0.2*80+0.2*160*i+posx,0.2*640+posy,10));
        positions.push_back(glm::vec3(0.2*160*i+posx,0.2*640+posy,10));

        colors.push_back(colorPurple1);
        colors.push_back(colorPurple1);
        colors.push_back(colorPurple1);
        colors.push_back(colorPurple1);

        drawTriangle(positions,colors,0);
    }

    //横着的
    for(int i=0;i<4;i++){
        vector<glm::vec3> positions;
        vector<glm::vec3> colors;
        
        positions.push_back(glm::vec3(0+posx,0.2*80+0.2*160*i+posy,0));
        positions.push_back(glm::vec3(0+posx,0.2*160+0.2*160*i+posy,0));
        positions.push_back(glm::vec3(0.2*640+posx,0.2*160+0.2*160*i+posy,0));
        positions.push_back(glm::vec3(0.2*640+posx,0.2*80+0.2*160*i+posy,0));

        colors.push_back(colorBlue);
        colors.push_back(colorBlue);
        colors.push_back(colorBlue);
        colors.push_back(colorBlue);

        drawTriangle(positions,colors,0.2*160);
        hw3_flag = !hw3_flag;

    }
    // hw3_flag = !hw3_flag;

    //横着的2号
    for(int i=0;i<4;i++){
        vector<glm::vec3> positions;
        vector<glm::vec3> colors;
        
        positions.push_back(glm::vec3(0+posx,0.2*160*i+posy,0));
        positions.push_back(glm::vec3(0+posx,0.2*80+0.2*160*i+posy,0));
        positions.push_back(glm::vec3(0.2*640+posx,0.2*80+0.2*160*i+posy,0));
        positions.push_back(glm::vec3(0.2*640+posx,0.2*160*i+posy,0));

        colors.push_back(colorBlue1);
        colors.push_back(colorBlue1);
        colors.push_back(colorBlue1);
        colors.push_back(colorBlue1);
        drawTriangle(positions,colors,0.2*80);
        hw3_flag = !hw3_flag;

    }


    for(int i=0;i<SCR_WIDTH;i++)
        for(int j=0;j<SCR_HEIGHT;j++)
            this->zBuffer[i][j]=-1000000;
}