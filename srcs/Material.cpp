#include "Material.h"

Material::Material()
{
	specularIntensity = 0;
	shininess = 0;
}

Material::Material(GLfloat specularIntensity, GLfloat shininess)
{
	this->specularIntensity = specularIntensity;
	this->shininess = shininess;
}

void Material::UseMaterial(Shader *shader)
{
	glUniform1f(shader->GetSpecularIntensityLocation(), specularIntensity);
	glUniform1f(shader->GetShininessLocation(), shininess);
}

void Material::ChangeMaterial(GLfloat specularIntensity, GLfloat shininess)
{
	this->specularIntensity = specularIntensity;
	this->shininess = shininess;
}

void Material::UnuseMaterial(Shader* shader)
{
	glUniform1f(shader->GetSpecularIntensityLocation(), 0);
	glUniform1f(shader->GetShininessLocation(), 0);
}

Material::~Material() {}
