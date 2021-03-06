#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light(), direction((0.0f, -1.0f, 0.0f)) {};

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientIntensity,
								   GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat diffuseIntensity,
								   GLuint shadowWidth, GLuint shadowHeight)
								   : Light(red, green, blue, ambientIntensity, diffuseIntensity, shadowWidth, shadowHeight),
									direction(glm::normalize(glm::vec3(xDir, yDir, zDir)))
{
	shadowMap = new ShadowMap();
	shadowMap->Init(shadowWidth, shadowHeight);
	lightProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, 0.1f, 100.0f);
};

void DirectionalLight::UseLight(GLuint ambientIntensityLocation, GLuint colorLocation, GLuint diffuseIntensityLocation, GLuint directionLocation,
	GLfloat ambientIntensity, GLfloat diffuseIntensity, glm::vec3 color, glm::vec3 direction)
{
	this->direction = direction;
	Light::UseLight(ambientIntensityLocation, colorLocation, diffuseIntensityLocation, ambientIntensity, diffuseIntensity, color);
	glUniform3f(directionLocation, this->direction.x, this->direction.y, this->direction.z);
}

glm::mat4 DirectionalLight::CalculateLightSpaceTransform()
{
	if (direction.x == 0.0f && direction.z == 0.0f)
		return lightProjection * glm::lookAt(-direction + glm::vec3(10.0, 0.0, -10.0), glm::vec3(10.0f, 0.0f, -10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	else
		return lightProjection * glm::lookAt(-direction + glm::vec3(10.0, 0.0, -10.0), glm::vec3(10.0f, 0.0f, -10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

DirectionalLight::~DirectionalLight()
{
	if (shadowMap)
	{
		delete shadowMap;
		shadowMap = nullptr;
	}
}
