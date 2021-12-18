#pragma once

#include <GL/glew.h>

class Mesh
{
public:
	Mesh();

	void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices,
					unsigned int numOfIndices);
	void RenderMesh();
	void ClearMesh();	// to free the memory of GPU
	~Mesh();

private:
	GLuint VertexBufferOjbect, VertexArrayObject, IndexBufferObject;
	GLsizei indexCount;

};

