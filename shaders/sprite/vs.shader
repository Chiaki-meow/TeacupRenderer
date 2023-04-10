#version 330 core
layout (location = 0) in vec4 aPos; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos.zw;
//     gl_Position =vec4(vertex.xy, 0.0, 1.0);
    gl_Position = projection * view * model * vec4(aPos.xy, 0.0, 1.0);
}