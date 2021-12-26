#include "Shader.h"


Shader::Shader()
{
	shaderID = 0;
	uniformModelMatrix = 0;
	uniformProjectionMatrix = 0;
}

void Shader::CreateShaderFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateShaderFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	CompileShader(vertexCode, fragmentCode);

}

std::string Shader::ReadFile(const char* fileLocation)
{
	std::string content;	// the string to store all shader codes.

	std::ifstream fileStream(fileLocation, std::ios::in);	//Open for input operations.

	if (!fileStream.is_open())
	{
		printf("Failed to read %s! File may not exist.", fileLocation);
		return "";
	}
	std::string line = "";
	while (!fileStream.eof())	// eof() Returns true if a file open for reading has reached the end.
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

GLuint Shader::GetProjectionLocation()
{
	return uniformProjectionMatrix;
}

GLuint Shader::GetModelLocation()
{
	return uniformModelMatrix;
}

GLuint Shader::GetViewLocation()
{
	return uniformViewMatrix;
}

void Shader::UseShader()
{
	glUseProgram(shaderID);
}

void Shader::ClearShader()
{
	if (shaderID != 0)
	{
		glDeleteProgram(shaderID);
		shaderID = 0;
	}
	uniformModelMatrix = 0;
	uniformProjectionMatrix = 0;
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
	shaderID = glCreateProgram();
	if (!shaderID)
	{
		printf("Error creating shader program!\n");
		return;
	}

	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	printf("Added vertex shader.\n");
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);
	printf("Added fragment shader.\n");

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

	uniformModelMatrix = glGetUniformLocation(shaderID, "ModelMatrix");
	uniformProjectionMatrix = glGetUniformLocation(shaderID, "ProjectionMatrix");
	uniformViewMatrix = glGetUniformLocation(shaderID, "ViewMatrix");	// I forgot this line, and spent 2 hours to debug it!!
}

void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
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

Shader::~Shader()
{
	ClearShader();
}