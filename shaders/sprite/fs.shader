#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform vec3 uniColor;
uniform sampler2D image;


void main()
{
//     color = vec4(uniColor, 1.0);
    color = texture(image, TexCoords);

}