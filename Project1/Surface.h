#pragma once

#include <GL/glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "ObjectLoader.h"

/*
*Surfaces are just tiled surface.obj's arranged as children of a parent transform at the center of the full Surface
*/
class Surface
{
public:
	//takes in a model matrix for the 'parent' transform, the center of the surface, the size of a tile and the number of tiles. number of tiles should be a square number (e.g. 9 or 16)
	Surface(GLuint MatrixID, GLuint ModelMatrixID, const char* pathToObject);

	void Render(glm::mat4 parentTransform, glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

private:
	std::vector<glm::vec3> translations;
	GLuint matrixID;
	GLuint modelMatrixID;

	ObjectLoader objectLoader;
	GLuint vertexBuffer;
	GLuint normalBuffer;
	GLuint elementBuffer;

	std::vector<unsigned short> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
};