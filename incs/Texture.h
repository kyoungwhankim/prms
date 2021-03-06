#pragma once

#include "GL/glew.h"
#include "Values.h"

class Texture
{
public:
	Texture();
	Texture(const char* filePath);
	~Texture();
				
	bool		LoadTexture();
	void		UseTexture();
	void		ClearTexture();

private:
	GLuint		textureID;
	int			width, height, bitDepth;
	const char*	filePath;
};


