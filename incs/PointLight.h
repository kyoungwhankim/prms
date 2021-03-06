#pragma once

#include <vector>
#include "Light.h"
#include "OmniShadowMap.h"

class PointLight : public Light
{
public:
	PointLight();
	PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientIntensity, GLfloat diffuseIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos, GLfloat constant, GLfloat linear, GLfloat exponent,
		GLuint shadowWidth, GLuint shadowHeight, GLfloat nearPlane, GLfloat farPlane);
	~PointLight();
	
	void					UseLight(GLuint ambientIntensityLocation, GLuint colorLocation,
									GLuint diffuseIntensityLocation, GLuint positionLocation,
									GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
									GLfloat ambientIntensity, GLfloat diffuseIntensity, glm::vec3 color, glm::vec3 position);

	std::vector<glm::mat4>	CalculateLightMatrices();

	glm::vec3				GetPosition() { return position; };
	GLfloat					GetNearPlane() { return nearPlane; };
	GLfloat					GetFarPlane() { return farPlane; };

protected:
	glm::vec3				position;
	GLfloat					constant, linear, exponent;
	GLfloat					nearPlane, farPlane;
	glm::mat4				lightProjection;
};

