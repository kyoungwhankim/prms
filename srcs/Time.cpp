#include "Time.h"

Time::Time()
{
	now = 0.0f;
	deltaTime = 0.0f;
	lastTime = 0.0f;
}

GLfloat Time::getDeltaTime()
{
	now = glfwGetTime();
	deltaTime = now - lastTime;
	lastTime = now;

	return deltaTime;
}

Time::~Time() {};