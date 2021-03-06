#include "Window.h"

Window::Window()
{
	width = 80;
	height = 60;

    for (int i = 0; i < 1024; ++i)
        keys[i] = 0;

    mainWindow = NULL;
    bufferHeight = 0;
    bufferWidth = 0;
    lastX = 10;
    lastY = 0;
    xChange = 0;
    yChange = 0;
    mouseFirstMoved = true;
}

Window::Window(GLint width, GLint height)
{
	this->width = width;
	this->height = height;

    for (int i = 0; i < 1024; ++i)
        keys[i] = 0;

    mainWindow = NULL;
    bufferHeight = 0;
    bufferWidth = 0;
    lastX = 0;
    lastY = 0;
    xChange = 0;
    yChange = 0;
    mouseFirstMoved = true;
}

int Window::initWindow()
{
    /*
    -----------------------------GLFW-------------------------------
    */
    if (!glfwInit())
    {
        printf("Error: Cannot init GLFW.");
        glfwTerminate();
        return -1;
    }

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWmonitor* primary = glfwGetPrimaryMonitor();

    mainWindow = glfwCreateWindow(width, height, "Tony's Phong Reflection Model Simulator", NULL, NULL);
    if (!mainWindow)
    {
        printf("Error: Cannot create window.");
        glfwTerminate();
        return -1;
    }
    glfwSetWindowUserPointer(mainWindow, static_cast<void*>(this));

    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
    glViewport(0, 0, bufferWidth, bufferHeight);

    glfwMakeContextCurrent(mainWindow);

    createCallbacks();

    //glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSwapInterval(1); 
    /*
    void glfwSwapInterval(int interval)

    This function sets the swap interval for the current OpenGL or OpenGL ES context, 
    i.e. the number of screen updates to wait from the time glfwSwapBuffers was called 
    before swapping the buffers and returning.
    Should always be called when a context is current on the calling thread.
    */

    /*
    -----------------------------GLEW-------------------------------
    */

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) // GLEW should be initialized after creating OpenGL context because GLEW uses it
    {
        printf("Error: Cannot init GLEW.");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    return 0;
}

GLfloat	Window::getXChange()
{
    GLfloat change = xChange;
    xChange = 0;

    return change;
}
GLfloat	Window::getYChange()
{
    GLfloat change = yChange;
    yChange = 0;

    return change;
}

void Window::createCallbacks()
{
    /* 
    If you want to be notified or execute actions when a key is pressed or a cursor is moved,
    you can use Callback functions.
    */

    glfwSetKeyCallback(mainWindow, handleKeys);
    glfwSetCursorPosCallback(mainWindow, handleMouse);
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    /*
    void* glfwGetWindowUserPointer(GLFWwindow* window)
    -> This function returns the current value of the user-defined pointer of the specified window.

    If you are trying to use glfw in a program that follows object-oriented design, for example, 
    you can use this pointer to store the address of the instance that is handling a particular window, 
    and forward the callbacks to the appropriate member functions.
    */

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            theWindow->keys[key] = true;
        else if (action == GLFW_RELEASE)
            theWindow->keys[key] = false;
    }
}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    /*
    void* glfwGetWindowUserPointer(GLFWwindow* window)
    -> This function returns the current value of the user-defined pointer of the specified window.

    If you are trying to use glfw in a program that follows object-oriented design, for example,
    you can use this pointer to store the address of the instance that is handling a particular window,
    and forward the callbacks to the appropriate member functions.
    */

    /*if (theWindow->mouseFirstMoved)
    {
        theWindow->lastX = xPos;
        theWindow->lastY = yPos;
        theWindow->mouseFirstMoved = false;
    }*/

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        theWindow->xChange = xPos - theWindow->lastX;
        theWindow->yChange = theWindow->lastY - yPos;
    }
    theWindow->lastX = xPos;
    theWindow->lastY = yPos;
}

Window::~Window()
{
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}