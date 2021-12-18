#include "Mesh.h"

Mesh::Mesh()
{
	VertexArrayObject = 0;
	VertexBufferOjbect = 0;
	IndexBufferObject = 0;
	indexCount = 0;
}

void Mesh::CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices)
{

	// Create and bind vertex array object.
	glGenVertexArrays(1, &VertexArrayObject);	// 1 means one array
	glBindVertexArray(VertexArrayObject);

	// create buffer, bind data with indices. 
	glGenBuffers(1, &IndexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, // indices is a pointer, so sizeof(indices) is not the length of the relative array
				indices, GL_STATIC_DRAW);

	
	// Create and bind vertex buffer object
	glGenBuffers(1, &VertexBufferOjbect);	// 1 means one buffer
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferOjbect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices ,
				vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);	// the first 0 is the location in the shader
	glEnableVertexAttribArray(0);	// 0 is same with the first 0 above.

	// unbind IBO buffer after VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}

void Mesh::RenderMesh()
{
	glBindVertexArray(VertexArrayObject);
	//glDrawArrays(GL_TRIANGLES, 0, 3);	// 0 is the first element of array, 3 means three vertices.

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferObject);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::ClearMesh()
{

}
