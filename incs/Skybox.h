#pragma once

#include <vector>
#include <string>
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include "Values.h"
#include "Mesh.h"
#include "Shader.h"

class Skybox
{
public:
	Skybox() : skyMesh(nullptr), skyShader(nullptr), textureID(0), uniformProjection(0), uniformView(0) {};
	Skybox(const std::vector<const char*>& facePaths);
	~Skybox();

	void	DrawSkybox(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

private:
	Mesh*	skyMesh;
	Shader*	skyShader;

	GLuint	textureID;
	GLuint	uniformProjection, uniformView;

};

