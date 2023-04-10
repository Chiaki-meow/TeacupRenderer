#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D sprite;
uniform vec4 ParticleColor;

void main()
{
    color = texture(sprite, TexCoords) * ParticleColor;
}