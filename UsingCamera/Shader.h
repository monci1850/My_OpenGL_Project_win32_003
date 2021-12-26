#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>


#include <GL\glew.h>

class Shader
{
public:
	Shader();

	void CreateShaderFromString(const char* vertexCode, const char* fragmentCode);
	void CreateShaderFromFiles(const char* vertexLocation, const char* fragmentLocation);

	std::string ReadFile(const char* fileLocation);

	GLuint GetProjectionLocation();
	GLuint GetModelLocation();
	GLuint GetViewLocation();

	void UseShader();
	void ClearShader();

	~Shader();

private:
	GLuint shaderID, uniformProjectionMatrix, uniformModelMatrix, uniformViewMatrix;

	/**
	 * Compile shader in Shader constructor. 
     * Don't forget to get uniform locations for all matrices.
	 */
	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};

