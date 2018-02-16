#include "Surface.h"

Surface::Surface(GLuint MatrixID, GLuint ModelMatrixID, const char* pathToObject) : matrixID(MatrixID), modelMatrixID(ModelMatrixID)
{

	translations.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	translations.push_back(glm::vec3(2.0f, 0.0f, 0.0f));
	translations.push_back(glm::vec3(-2.0f, 0.0f, 0.0f));
	translations.push_back(glm::vec3(0.0f, 0.0f, 2.0f));
	translations.push_back(glm::vec3(0.0f, 0.0f, -2.0f));
	translations.push_back(glm::vec3(2.0f, 0.0f, 2.0f));
	translations.push_back(glm::vec3(2.0f, 0.0f, -2.0f));
	translations.push_back(glm::vec3(-2.0f, 0.0f, 2.0f));
	translations.push_back(glm::vec3(-2.0f, 0.0f, -2.0f));

	const char * path = pathToObject;
	objectLoader.LoadObjectFile(path, indices, vertices, uvs, normals);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
}

void Surface::Render(glm::mat4 parentTransform, glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	glm::mat4 MVP;
	glm::mat4 modelMatrix;
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

	for each (glm::vec3  translation in translations)
	{
		modelMatrix = glm::translate(parentTransform, translation);
		MVP = projectionMatrix * viewMatrix * modelMatrix;
		glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);

		glDrawElements(
			GL_TRIANGLES,
			indices.size(),
			GL_UNSIGNED_SHORT,
			(void*)0
		);
	}
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}
