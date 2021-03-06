#pragma once

#include <stdio.h>
#include "GL/glew.h"

class ShadowMap
{
public:
	ShadowMap() : shadowMap(0), FBO(0), shadowWidth(0), shadowHeight(0) {};
	~ShadowMap();

	virtual bool	Init(GLuint shadowWidth, GLuint shadowHeight);
	virtual void	Write();
	virtual void	Read(GLenum textureUnit);

	GLuint			GetShadowWidth() { return shadowWidth; };
	GLuint			GetShadowHeight() { return shadowHeight; };

protected:
	GLuint			FBO, shadowMap;
	GLuint			shadowWidth, shadowHeight;
};

