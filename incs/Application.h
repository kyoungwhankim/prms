#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include <stdio.h>
#include <string.h>
#include <vector>
#include <assimp/Importer.hpp>
#include <crtdbg.h>

#include "Mesh.h"
#include "Shader.h" // includes "DirectionalLight.h", "SpotLight.h"
#include "Window.h"
#include "Camera.h"
#include "Time.h"
#include "Texture.h"
#include "Material.h"
#include "Model.h"
#include "Skybox.h"
#include "MyGui.h"
#include "Values.h"

class Application
{
public:
	Application();
	~Application();

	bool						AppRun();

private:
	Window						mainWindow;
	Camera						camera;
	Camera						front_camera;
	Camera						left_camera;
	Camera						right_camera;
	Camera						top_camera;
	Camera						back_camera;
	std::vector<Mesh*>			meshList;
	glm::mat4					projection;
	Shader						mainShader;
	Shader						directionalShadowShader;
	Shader						omniShadowShader;
	Texture						grid;
	Time						time;
	DirectionalLight*			mainLight;
	PointLight*					pointLights[MAX_POINT_LIGHTS];
	SpotLight*					spotLights[MAX_SPOT_LIGHTS];
	unsigned int				spotLightCount;
	unsigned int				pointLightCount;
	Material					metal;
	Material					mat;
	Model						couch_body;
	Model						couch_cushions1;
	Model						couch_cushions2;
	Model						sphere;
	Model						box;
	Skybox*						castle;
	Skybox*						sunsetLake;
	Skybox*						clearOcean;
	std::vector<const char*>	skyboxFaces;
	MyGui						myGui;

	// Shader Paths
	const char*					vShader = "shaders/shader.vert";
	const char*					fShader = "shaders/shader.frag";
	const char*					vDirectionalShadowShader = "shaders/directional_shadow_map.vert";
	const char*					fDirectionalShadowShader = "shaders/directional_shadow_map.frag";
	const char*					vOmniShadowShader = "shaders/omni_shadow_map.vert";
	const char*					gOmniShadowShader = "shaders/omni_shadow_map.geom";
	const char*					fOmniShadowShader = "shaders/omni_shadow_map.frag";

	// Method Functions
	void						calculateAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
														unsigned int iLength, unsigned int vLength, unsigned int normalOffset);
	void						CreateObjects();
	void						CreateShaders();
	void						RenderScene(Shader* shader);
	void						DirectionalShadowMapPass(DirectionalLight* light);
	void						OmniShadowMapPass(PointLight* light);
	void						RenderPass(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix);
	void						SetCamera();
};
