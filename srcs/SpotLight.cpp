#include "SpotLight.h"

void SpotLight::UseLight(GLuint ambientIntensityLocation, GLuint colorLocation, GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation, GLuint directionLocation, GLuint edgeLocation)
{
	Light::UseLight(ambientIntensityLocation, colorLocation, diffuseIntensityLocation, ambientIntensity, diffuseIntensity, color);
	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);
	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(edgeLocation, procEdge);
}

void SpotLight::SetFlash(const glm::vec3& position, const glm::vec3& direction)
{
	this->position = position;
	this->direction = direction;
}

void SpotLight::FlashOn()
{
	ambientIntensity = diffuseIntensity = 1.0f;
}

void SpotLight::FlashOff()
{
	ambientIntensity = diffuseIntensity = 0.0f;
}