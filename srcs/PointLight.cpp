#include "PointLight.h"

PointLight::PointLight() : Light(), position(glm::vec3(0, 0, 0)), constant(1.0f), linear(0.0f), exponent(0.0f), 
							farPlane(0.0f), nearPlane(0.0f), lightProjection(glm::mat4(1.0f)) {};

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientIntensity, GLfloat diffuseIntensity,
	GLfloat xPos, GLfloat yPos, GLfloat zPos, GLfloat constant, GLfloat linear, GLfloat exponent,
	GLuint shadowWidth, GLuint shadowHeight, GLfloat nearPlane, GLfloat farPlane)
	: Light(red, green, blue, ambientIntensity, diffuseIntensity, shadowWidth, shadowHeight),
	position(glm::vec3(xPos, yPos, zPos)), constant(constant), linear(linear), exponent(exponent),
	nearPlane(nearPlane), farPlane(farPlane) 
{
	float aspect = (float)shadowWidth / (float)shadowHeight;
	lightProjection = glm::perspective(glm::radians(90.0f), aspect, nearPlane, farPlane);

	shadowMap = new OmniShadowMap();
	shadowMap->Init(shadowWidth, shadowHeight);
};

void PointLight::UseLight(GLuint ambientIntensityLocation, GLuint colorLocation, GLuint diffuseIntensityLocation, 
						GLuint positionLocation, GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
						GLfloat ambientIntensity, GLfloat diffuseIntensity, glm::vec3 color, glm::vec3 position)
{
	this->position = position;
	Light::UseLight(ambientIntensityLocation, colorLocation, diffuseIntensityLocation, ambientIntensity, diffuseIntensity, color);
	glUniform3f(positionLocation, this->position.x, this->position.y, this->position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);
}

std::vector<glm::mat4> PointLight::CalculateLightMatrices()
{
	std::vector<glm::mat4> lightMatrices;

	lightMatrices.push_back(lightProjection * glm::lookAt(position, position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	lightMatrices.push_back(lightProjection * glm::lookAt(position, position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	lightMatrices.push_back(lightProjection * glm::lookAt(position, position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	lightMatrices.push_back(lightProjection * glm::lookAt(position, position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
	lightMatrices.push_back(lightProjection * glm::lookAt(position, position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	lightMatrices.push_back(lightProjection * glm::lookAt(position, position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

	return lightMatrices;
}

PointLight::~PointLight()
{
	if (shadowMap)
	{
		delete shadowMap;
		shadowMap = nullptr;
	}
}