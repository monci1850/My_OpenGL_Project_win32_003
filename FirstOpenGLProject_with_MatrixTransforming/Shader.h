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

	GLuint GetProjectionLocation() const;
	GLuint GetModelLocation() const;
	
	void UseShader();
	void ClearShader();

	~Shader();

private:
	GLuint shaderID, uniformProjectionMatrix, uniformModelMatrix;

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};

