#include "Light.h"

void Light::UseLight(GLuint ambientIntensityLocation, GLuint colorLocation, GLuint diffuseIntensityLocation,
	GLfloat ambientIntensity, GLfloat diffuseIntensity, glm::vec3 color)
{
	this->ambientIntensity = ambientIntensity;
	this->diffuseIntensity = diffuseIntensity;
	this->color = color;
	glUniform1f(ambientIntensityLocation, this->ambientIntensity);
	glUniform3f(colorLocation, this->color.x, this->color.y, this->color.z);
	glUniform1f(diffuseIntensityLocation, this->diffuseIntensity);
}

Light::~Light() {}