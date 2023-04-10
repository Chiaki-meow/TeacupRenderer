#version 330 core
out vec4 FragColor;

uniform vec3 uniColor;
uniform sampler2D image;

in vec2 TexCoords;


void main()
{
    FragColor = vec4(uniColor, 1.0f);
//     FragColor = texture(image, TexCoords);
}