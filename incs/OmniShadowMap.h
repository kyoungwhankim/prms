#pragma once
#include "ShadowMap.h" 

class OmniShadowMap : public ShadowMap
{
public:
	OmniShadowMap() : ShadowMap() {};
	~OmniShadowMap();

	bool	Init(GLuint shadowWidth, GLuint shadowHeight);
	void	Write();
	void	Read(GLenum textureUnit);
};

