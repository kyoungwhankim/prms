#pragma once

#include <GL/glew.h>
#include "Shader.h"

class Material
{
public:
	Material();
	Material(GLfloat specularIntensity, GLfloat shininess);
	~Material();

	void	UseMaterial(Shader* shader);
	void	ChangeMaterial(GLfloat specularIntensity, GLfloat shininess);
	void	UnuseMaterial(Shader* shader);

private:
	GLfloat specularIntensity;
	GLfloat shininess;
};

