#pragma once

#include <GL/glew.h>
#include <GLM/glm.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Values.h"

class MyGui
{
public:
	MyGui();
	~MyGui();
	
	void	HelpMarker(const char* desc);
	void	ShowAbout(bool* p_open);
	void	ShowHelp(bool* p_open);
	void	ShowLicense(bool* p_open);

	void	CreateMainMenu();
	void	CollapsingHeader_Camera();
	void	CollapsingHeader_Mesh();
	void	CollapsingHeader_Light();
	void	CollapsingHeader_Ground();
	void	CollapsingHeader_Skybox();

	void	ActivateGui();

	ImGuiWindowFlags	window_flags;
	bool				p_open;
	bool				show_about;
	bool				show_help;
	bool				show_license;
	int					current_skybox;
	int					current_mesh_type;
	int					current_camera;
	float				mesh_rotationX;
	float				mesh_rotationY;
	float				mesh_rotationZ;
	glm::vec3			mesh_color;
	GLfloat				mesh_specular;
	GLfloat				mesh_shininess;
	GLfloat				d_ambientIntensity, d_diffuseIntensity;
	glm::vec3			d_color, d_direction;
	GLfloat				p_ambientIntensity[MAX_POINT_LIGHTS], p_diffuseIntensity[MAX_POINT_LIGHTS];
	glm::vec3			p_color[MAX_POINT_LIGHTS], p_position[MAX_POINT_LIGHTS];
	bool				flash_activated;
	bool				show_ground;

	GLfloat				ground_specular;
	GLfloat				ground_shininess;

};

