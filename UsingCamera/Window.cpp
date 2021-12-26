#include "Window.h"

Window::Window()
{
	height = 600;
	width = 600;

	// Check the list of all keys, 
	// is to initialize all the keys to released state
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = false;
	}

	/**
	 *A common issue people have with the following lesson, is that the camera sometimes starts off facing in a random direction.
	 *This is actually a bug in the code in the lesson video! I neglected to set the "xChange" and "yChange" to 0.0f in the Window constructor, so it starts off with a garbage value for the x/y change and instantly offsets the camera.
	 *To fix this, simply set xChange and yChange to 0.0f in the two constructors for the Window class (in Window.cpp) like so:
	 *xChange = 0.0f;
	 *yChange = 0.0f;
	 *This should fix any camera issues that crop up.
	 */
	xChange = 0.0f;
	yChange = 0.0f;
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	height = windowHeight;
	width = windowWidth;

	// Check the list of all keys, 
	// is to initialize all the keys to released state
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = false;
	}

	// Same reason with the code in default constructor.
	xChange = 0.0f;
	yChange = 0.0f;
}

int Window::Initialize()
{
	// Initialize GLFW
	if (!glfwInit())
	{
		printf("GLFW initialization failed!");
		glfwTerminate();
		return 1;
	}

	// Setup GLFW window properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core Profile = No Backwards Compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow Forward Compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create the window
	mainWindow = glfwCreateWindow(width, height, "Hello OpenGL Window", NULL, NULL);
	if (!mainWindow)
	{
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// Get Buffer Size information, this is actually not necessary for showing up the window.
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);	// use reference to put values in.

	// Set context for GLEW to use. 
	glfwMakeContextCurrent(mainWindow);

	// Handle Key + Mouse Input
	createCallbacks();

	// Hide the cursor
	//glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Allow modern extension features, try to access GLEW
	glewExperimental = GL_TRUE;

	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		printf("GLEW initialization failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Enable Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Setup Viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	// to tell the static function that GLFW called which is the current window.
	// "this" means the user of this window.
	glfwSetWindowUserPointer(mainWindow, this);
}


void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, handleKeys);
	glfwSetCursorPosCallback(mainWindow, handleMouse);
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			printf("Pressed key: %d\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			printf("Released key: %d\n", key);
		}
	}
}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;  // inverted Y axis

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;

	// debug x y change
	//printf("x:%.6f, y:%.6f\n", theWindow->xChange, theWindow->yChange);

	// debug xPos yPos
	//printf("x:%.6f, y:%.6f\n", xPos, yPos);
}

GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}