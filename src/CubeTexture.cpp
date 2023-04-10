#include "CubeTexture.h"

CubeTexture::CubeTexture()
	: Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB),
	Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR)
{
    // Create Texture
	glGenTextures(1, &this->ID);
}

CubeTexture::~CubeTexture()
{
	//	glDeleteTextures(1, &this->ID);
}

void CubeTexture::Generate(GLuint width, GLuint height, const vector<unsigned char*>& data, GLenum format)
{
	this->Width = width;
	this->Height = height;
	this->Internal_Format = format;

    glBindTexture(GL_TEXTURE_CUBE_MAP, this->ID);
    for (unsigned int i = 0; i < data.size(); i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, this->Internal_Format, 
            width, height, 0, this->Internal_Format, GL_UNSIGNED_BYTE, data[i]);
    }
    // Set Texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void CubeTexture::Bind() const {
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->ID);
}