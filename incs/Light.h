#pragma once

#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include "ShadowMap.h"

class Light
{
public:
	Light() : color((1.0f, 1.0f, 1.0f)), ambientIntensity(1.0f), diffuseIntensity(1.0f), shadowMap(nullptr), lightProjection(glm::mat4(1.0f)) {};
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientIntensity, GLfloat diffuseIntensity, GLuint shadowWidth, GLuint shadowHeight)
		: color(red, green, blue), ambientIntensity(ambientIntensity), diffuseIntensity(diffuseIntensity), shadowMap(nullptr), lightProjection(glm::mat4(1.0f)) {};
	~Light();

	void		UseLight(GLuint ambientIntensityLocation, GLuint colorLocation, GLuint diffuseIntensityLocation,
		GLfloat ambientIntensity, GLfloat diffuseIntensity, glm::vec3 color);
	ShadowMap*	GetShadowMap() { return shadowMap; }

protected:
	glm::vec3	color;
	GLfloat		ambientIntensity;
	GLfloat		diffuseIntensity;
	ShadowMap*	shadowMap;
	glm::mat4	lightProjection;
};

