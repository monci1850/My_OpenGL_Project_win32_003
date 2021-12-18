/* This project is to test the projection matrix.
* Q:? How to use time module to check the meaning of
* "glm::mat4 myProjectionMatrixValue = glm::perspective(glm::radians(70.0f), "
*/
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

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


#include "mesh.h"
#include "Shader.h"
#include "Window.h"

Window mainWindow;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

// location of shaders
static const char* vertexShaderLocation = "Shaders/shader.vert";
static const char* fragmentShaderLocation = "Shaders/shader.frag";
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


void CreatPyramid();
void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
void CreateShaders();


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialize();


	CreatPyramid();
	CreateShaders();

	// Create projection matrix outside while loop to optimize performance.
	glm::mat4 myProjectionMatrixValue = glm::perspective(glm::radians(70.0f),
										(GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(), 0.1f, 100.f);

	// Loop until window closed
	while (!mainWindow.getShouldClose())
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
		shaderList[0].UseShader();
		uniformModelMatrix = shaderList[0].GetModelLocation();
		uniformProjectionMatrix = shaderList[0].GetProjectionLocation();

		// Compute translation and rotation matrix and assign it.
		glm::mat4 myModelMatrixValue(1.0f);
		myModelMatrixValue = glm::translate(myModelMatrixValue, glm::vec3(0.0f, 0.0f, -3.f));
		myModelMatrixValue = glm::rotate(myModelMatrixValue, currentAngle * toRadians1Degree,
							 glm::vec3(0.f, 1.f, 0.f));
		myModelMatrixValue = glm::scale(myModelMatrixValue, glm::vec3(0.4f, 0.4f, 1.f));
		glUniformMatrix4fv(uniformModelMatrix, 1, GL_FALSE, glm::value_ptr(myModelMatrixValue));
		glUniformMatrix4fv(uniformProjectionMatrix, 1, GL_FALSE, glm::value_ptr(myProjectionMatrixValue));

		meshList[0]->RenderMesh();

		// Compute translation and scaling matrix and assign it.
		myModelMatrixValue = glm::mat4(1.0f);
		myModelMatrixValue = glm::translate(myModelMatrixValue, glm::vec3(0.0f, 1.0f, -3.f));
		myModelMatrixValue = glm::rotate(myModelMatrixValue, currentAngle * toRadians1Degree,
							 glm::vec3(0.f, 1.f, 0.f));
		myModelMatrixValue = glm::scale(myModelMatrixValue, glm::vec3(0.4f, 0.4f, 1.f));
		glUniformMatrix4fv(uniformModelMatrix, 1, GL_FALSE, glm::value_ptr(myModelMatrixValue));
		meshList[1]->RenderMesh();


		// Unassigns the shader ID.
		glUseProgram(0);

		/* Use Shader End*/
		mainWindow.swapBuffers();
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

	// the first one
	Mesh *geo1 = new Mesh();
	geo1->CreateMesh(vertiecs, indices, 12, 12);
	meshList.push_back(geo1);

	// the second one
	Mesh* geo2 = new Mesh();
	geo2->CreateMesh(vertiecs, indices, 12, 12);
	meshList.push_back(geo2);

}

/**	Create new shader on the heap, and assign it into shader array. */
void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateShaderFromFiles(vertexShaderLocation, fragmentShaderLocation);
	shaderList.push_back(*shader1);
}