#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>

#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include "Values.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

class Shader
{
public:
	Shader();
	~Shader();

	void		CreateFromString(const char* vsCode, const char* fsCode);
	void		CreateFromFiles(const char* vsPath, const char* fsPath);
	void		CreateFromFiles(const char* vsPath, const char* gsPath, const char* fsPath);
	std::string	ReadFile(const char* filePath);

	void		Validate();

	GLuint		GetProjLocation() { return uniformProj; };
	GLuint		GetModelLocation() { return uniformModel; };
	GLuint		GetViewLocation() { return uniformView; };
	GLuint		GetShininessLocation() { return uniformShininess; };
	GLuint		GetSpecularIntensityLocation() { return uniformSpecularIntensity; };
	GLuint		GetEyePositionLocation() { return uniformEyePosition; };

	void		SetProjection(const glm::mat4& projection);
	void		SetView(const glm::mat4& view);
	void		SetModel(const glm::mat4& model);
	void		SetDirectionalLight(DirectionalLight* directionalLight, GLfloat ambientIntensity, 
									GLfloat diffuseIntensity, glm::vec3 color, glm::vec3 direction);
	void		SetPointLight(PointLight* pointLight[], unsigned int lightCount, unsigned int textureUnit, unsigned int offset,
							GLfloat ambientIntensity[MAX_POINT_LIGHTS], GLfloat diffuseIntensity[MAX_POINT_LIGHTS], glm::vec3 color[MAX_POINT_LIGHTS], glm::vec3 position[MAX_POINT_LIGHTS]);
	void		SetSpotLight(SpotLight* spotLight[], unsigned int lightCount, unsigned int textureUnit, unsigned int offset);
	void		SetColor(const glm::vec3& color);
	void		SetTexture(GLuint textureUnit);
	void		SetDirectionalShadowMap(GLuint textureUnit);
	void		SetDirectionalLightSpaceTransform(const glm::mat4& transform);
	void		SetLightMatrices(const std::vector<glm::mat4>& lightMatrices);
	void		SetOmniLightPos(const glm::vec3& position);
	void		SetNearPlane(GLfloat nearPlane);
	void		SetFarPlane(GLfloat farPlane);
	void		SetEyePosition(const glm::vec3& eyePosition);

	void		UseShader();
	void		ClearShader();

private:
	int			pointLightCount;
	int			spotLightCount;

	GLuint		shaderID;
	GLuint		uniformProj, uniformModel, uniformView, uniformEyePosition,
				uniformSpecularIntensity, uniformShininess, uniformPointLightCount, uniformSpotLightCount,
				uniformTexture, uniformDirectionalLightSpaceTransform, uniformDirectionalShadowMap,
				uniformOmniLightPos, uniformNearPlane, uniformFarPlane, uniformColor;
	GLuint		uniformLightMatrices[6];

	struct {
		GLuint	uniformColor;
		GLuint	uniformAmbientIntensity;
		GLuint	uniformDirection;
		GLuint	uniformDiffuseIntensity;
	} directionalLightUniforms;

	struct {
		GLuint	uniformColor;
		GLuint	uniformAmbientIntensity;
		GLuint	uniformDiffuseIntensity;
		GLuint	uniformPosition;
		GLuint	uniformConstant;
		GLuint	uniformLinear;
		GLuint	uniformExponent;
	} pointLightUniforms[MAX_POINT_LIGHTS];

	struct {
		GLuint	uniformColor;
		GLuint	uniformAmbientIntensity;
		GLuint	uniformDiffuseIntensity;
		GLuint	uniformPosition;
		GLuint	uniformConstant;
		GLuint	uniformLinear;
		GLuint	uniformExponent;
		GLuint	uniformDirection;
		GLuint	uniformEdge;
	} spotLightUniforms[MAX_SPOT_LIGHTS];

	struct {
		GLuint	uniformShadowMap;
		GLuint	uniformNearPlane;
		GLuint	uniformFarPlane;
	} omniShadowMapUniforms[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];

	void		CompileShader(const char* vsCode, const char* fsCode);
	void		CompileShader(const char* vsCode, const char* gsCode, const char* fsCode);
	GLuint		AddShader(const char* shaderCode, GLenum shaderType);

	void		CompileProgram();
};

