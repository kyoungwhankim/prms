#pragma once

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	Window();
	Window(GLint width, GLint height);

	~Window();

	GLFWwindow* mainWindow;

	int			initWindow();

	GLfloat		getBufferWidth() { return (GLfloat)bufferWidth; }
	GLfloat		getBufferHeight() { return (GLfloat)bufferHeight; }
	
	bool		getShouldClose() { return glfwWindowShouldClose(mainWindow); }
	
	bool*		getKeys() { return keys; }
	GLfloat		getXChange();
	GLfloat		getYChange();

	void		swapBuffers() { glfwSwapBuffers(mainWindow); }
		
private:
	GLint		width, height;
	GLint		bufferWidth, bufferHeight;

	// for keyboard input
	bool		keys[1024];

	// for mouse input
	GLfloat		lastX;
	GLfloat		lastY;
	GLfloat		xChange;
	GLfloat		yChange;
	bool		mouseFirstMoved;

	void		createCallbacks();
	static void	handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void	handleMouse(GLFWwindow* window, double xPos, double yPos);
};

