#pragma once
#include "PointLight.h"
class SpotLight : public PointLight
{
public:
	SpotLight() : PointLight(), direction(glm::vec3(0.0f, -1.0f, 0.0f)), edge(0.0f), procEdge(cosf(glm::radians(edge))) {};

	SpotLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientIntensity, GLfloat diffuseIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos, GLfloat constant, GLfloat linear, GLfloat exponent,
		GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat edge, GLuint shadowWidth, GLuint shadowHeight, GLfloat nearPlane, GLfloat farPlane)
		: PointLight(red, green, blue, ambientIntensity, diffuseIntensity, xPos, yPos, zPos, constant, linear, exponent,
			shadowWidth, shadowHeight, nearPlane, farPlane), direction(glm::normalize(glm::vec3(zDir, yDir, zDir))),
			edge(edge), procEdge(cosf(glm::radians(edge))) {};
	~SpotLight() {};

	void		UseLight(GLuint ambientIntensityLocation, GLuint colorLocation,
	 					GLuint diffuseIntensityLocation, GLuint positionLocation,
	   					GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
						GLuint directionLocation, GLuint edgeLocation);

	void		SetFlash(const glm::vec3& position, const glm::vec3& direction);
	void		FlashOn();
	void		FlashOff();

private:
	glm::vec3	direction;
	GLfloat		edge, procEdge;

};

