#pragma once

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <map>
#include <string>
#include <iomanip>


using namespace std;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const unsigned int MAX_VERTEX_LEN = 300;
const int MAX_INDICES_LEN = 600;
const int MAX_BEZIER_VERTEX_LEN = 3000;
const int MAX_CIRCLE_VERTEX_LEN = 50000;
const int MAX_CYLINDER_VERTEX_LEN = 600;
const float PI = 3.14f;
