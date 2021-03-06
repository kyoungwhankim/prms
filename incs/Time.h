#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Time
{
public:
	Time();
	~Time();

	GLfloat getDeltaTime();

private:
	GLfloat now;
	GLfloat deltaTime;
	GLfloat lastTime;
};

