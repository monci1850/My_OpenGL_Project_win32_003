#pragma once
#include "stdio.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);

	int Initialize();

	GLint getBufferWidth() const { return bufferWidth; } 
	GLint getBufferHeight() const { return bufferHeight; }

	bool getShouldClose() const { return glfwWindowShouldClose(mainWindow); }

	// sometimes, it has two or three buffers
	void swapBuffers() { glfwSwapBuffers(mainWindow); }

	~Window();

private:
	GLFWwindow* mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeight;
};

