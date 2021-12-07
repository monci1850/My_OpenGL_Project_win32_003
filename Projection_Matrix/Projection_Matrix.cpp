/* This project is to test the projection matrix.
* Q:? How to use time module to check the meaning of 
* "glm::mat4 myProjectionMatrixValue = glm::perspective(glm::radians(70.0f),... "
*/
#include <stdio.h>
#include <string.h>
#include <cmath>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>		/*The pointer to the actual location where GLM stores the
									* values that will be passed to the shader.
									*/


									/* Use the model matrix to covert from local space to world space.
									* In this case, we will get the same result with the example that using uniform value to
									* offset position in vertex shader. However, we will use model matrix instead of adding
									* offset directly in vertex shader vec4 output.
									*/



// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;

// Formula of degrees to radians 1¡ã ¡Á ¦Ð / 180 = 0.01745rad
const float toRadians1Degree = 3.14159265f / 180.f;
float currentAngle = 0.0f;

// Add projection matrix ID variable
GLuint VertexBufferOjbect, VertexArrayObject, IndexBufferObject,
		shader, uniformModelMatrix, uniformProjectionMatrix;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.001f;

// scaling related variables
bool scaleDirection = true;
float currentSize = 0.4f;
float maxSize = 0.5f;
float minSize = 0.05f;

// Vertex Shader code
static const char* vertexShader = "	\n\
#version 330                                                                  \n\
                                                                              \n\
layout (location = 0) in vec3 pos;											  \n\
out vec4 myColor;															  \n\
uniform	mat4 ModelMatrix;													  \n\
uniform	mat4 ProjectionMatrix;												  \n\
void main()                                                                   \n\
{                                                                             \n\
    gl_Position = ProjectionMatrix * ModelMatrix * vec4(pos, 1.0);	          \n\
	myColor = vec4(clamp(pos, 0.0f, 1.0f), 1.0);							  \n\
}";


// Fragment Shader
static const char* fragementShader = "                                        \n\
#version 330                                                                  \n\
in vec4 myColor;                                                              \n\
out vec4 colour;                                                              \n\
                                                                              \n\
void main()                                                                   \n\
{                                                                             \n\
    colour = myColor;														  \n\
}";


void CreatPyramid();
void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
void CompileShader();


int main()
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
	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Hello OpenGL Window", NULL, NULL);
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

	// Allow modern extension features, try to access GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
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

	CreatPyramid();
	CompileShader();

	// Create projection matrix outside while loop to optimize performance.
	glm::mat4 myProjectionMatrixValue = glm::perspective(glm::radians(70.0f), 
									(GLfloat)bufferWidth/(GLfloat)bufferHeight, 0.1f, 100.f);

	// Loop until window closed
	while (!glfwWindowShouldClose(mainWindow))
	{
		// Get + Handle user input events
		glfwPollEvents();

		// bouncing x move
		if (direction)
		{
			triOffset += triIncrement;
		}
		else
		{
			triOffset -= triIncrement;
		}
		if (abs(triOffset) >= triMaxOffset)
		{
			direction = !direction;
		}

		// accumulation rotation
		currentAngle += 1.0f;
		if (currentAngle >= 360)
		{
			currentAngle -= 360;
		}

		if (scaleDirection)
		{
			currentSize += 0.01f;
		}
		else
		{
			currentSize -= 0.01f;
		}


		if (currentSize >= maxSize || currentSize <= minSize)
		{
			scaleDirection = !scaleDirection;
		}



		// Clear window
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);	// RGBA
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// clear color buffer and depth buffer

		/* Use Shader Start*/

		// To use the one with the ID of the shader.
		glUseProgram(shader);

		// Compute translation and rotation matrix and assign it.
		glm::mat4 myModelMatrixValue(1.0f);
		myModelMatrixValue = glm::translate(myModelMatrixValue, glm::vec3(0.0f, -0.0f, -3.f));
		//myModelMatrixValue = glm::rotate(myModelMatrixValue, currentAngle * toRadians1Degree,
		//				glm::vec3(0.f, 1.f, 0.f));
		myModelMatrixValue = glm::scale(myModelMatrixValue, glm::vec3(0.4f, 0.4f, 1.f));


		glUniformMatrix4fv(uniformModelMatrix, 1, GL_FALSE, glm::value_ptr(myModelMatrixValue));
		glUniformMatrix4fv(uniformProjectionMatrix, 1, GL_FALSE, glm::value_ptr(myProjectionMatrixValue));

		//glUniform1f(uniformXMove, triOffset);	// assign uniform value

		glBindVertexArray(VertexArrayObject);
		//glDrawArrays(GL_TRIANGLES, 0, 3);	// 0 is the first element of array, 3 means three vertices.

		// Draw pyramid
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferObject);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		// Unassigns the shader ID.
		glUseProgram(0);

		/* Use Shader End*/

		glfwSwapBuffers(mainWindow);	// sometimes, it has two or three buffers
	}

	return 0;
}

void CreatPyramid()
{
	GLuint indices[] = {
		0,3,1,
		1,3,2,
		2,3,0,
		0,1,2
	};

	GLfloat vertiecs[] = {
		-1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &VertexArrayObject);	// 1 means one array
	glBindVertexArray(VertexArrayObject);

	// create buffer, bind data with indices. 
	glGenBuffers(1, &IndexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glGenBuffers(1, &VertexBufferOjbect);	// 1 means one buffer
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferOjbect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertiecs),
		vertiecs, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);	// the first 0 is the location in the shader
	glEnableVertexAttribArray(0);	// 0 is same with the first 0 above.

	// unbind IBO buffer after VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, 1024, NULL, eLog);
		fprintf(stderr, "Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);
}

void CompileShader()
{
	// Create the program, and give the global variable shader an ID.
	shader = glCreateProgram();
	if (!shader)
	{
		printf("Failed to create shader\n");
		return;

	}

	printf("vertex shader\n");
	AddShader(shader, vertexShader, GL_VERTEX_SHADER);
	printf("fragment shader\n");
	AddShader(shader, fragementShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

	uniformModelMatrix = glGetUniformLocation(shader, "ModelMatrix");
	uniformProjectionMatrix = glGetUniformLocation(shader, "ProjectionMatrix");
}
