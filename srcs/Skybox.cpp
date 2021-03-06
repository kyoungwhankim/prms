#include "Skybox.h"

Skybox::Skybox(const std::vector<const char*>& facePaths)
{
	skyShader			= nullptr;
	skyMesh				= nullptr;
	textureID			= 0;
	uniformProjection	= 0;
	uniformView			= 0;
	
	//Shader Setup
	skyShader = new Shader();
	skyShader->CreateFromFiles("shaders/skybox.vert", "shaders/skybox.frag");

	uniformProjection = skyShader->GetProjLocation();
	uniformView = skyShader->GetViewLocation();

	//Textures Setup
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, bitDepth;

	for (int i = 0; i < 6; i++)
	{
		unsigned char* texData = stbi_load(facePaths[i], &width, &height, &bitDepth, 4);
		if (!texData)
		{
			printf("Failed to find: %s\n", facePaths[i]);
			return;
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
		stbi_image_free(texData);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	//Mesh Setup
	unsigned int skyboxIndices[] = {
		// front
		0, 1, 2,
		2, 1, 3,
		// right
		2, 3, 5,
		5, 3, 7,
		// back
		5, 7, 4,
		4, 7, 6,
		// left
		4, 6, 0,
		0, 6, 1,
		// top
		4, 0, 5,
		5, 0, 2,
		// bottom
		1, 6, 3,
		3, 6, 7
	};

	float skyboxVertices[] = {
		-1.0f, 1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,

		-1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f
	};

	skyMesh = new Mesh;
	skyMesh->CreateMesh(skyboxVertices, skyboxIndices, 64, 36);
}

void Skybox::DrawSkybox(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	glDepthMask(GL_FALSE);

	skyShader->UseShader();

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	glActiveTexture(GL_TEXTURE0); //I want to set this to 1 by using another form of UseTexture from the texture object
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	skyShader->Validate();

	skyMesh->RenderMesh();

	glDepthMask(GL_TRUE);
}

Skybox::~Skybox() 
{
	if (skyShader)
	{
		delete skyShader;
		skyShader = nullptr;
	}
	if (skyMesh)
	{
		delete skyMesh;
		skyMesh = nullptr;
	}
};