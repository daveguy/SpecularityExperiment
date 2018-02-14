#include "LightsManager.h"

GLvoid* LightsManager::setUpLights(GLuint programID)
{
	lights = {
		{ glm::vec4(4,4,4,1), glm::vec4(1,1,1,50.0) },
		{ glm::vec4(-4,4,-4,1), glm::vec4(1,1,1,10.0) }
	};

	//Create a uniform buffer
	glGenBuffers(1, &lightBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
	glBufferData(GL_UNIFORM_BUFFER, lights.size() * sizeof(Light), &lights[0], GL_STATIC_DRAW);

	//get location in GPU memory and copy to there
	GLvoid* pointerToGPUMemory = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
	memcpy(pointerToGPUMemory, &lights[0], lights.size() * sizeof(Light));
	glUnmapBuffer(GL_UNIFORM_BUFFER);

	//Get the ID of the uniform block in the shader
	lightBlockID = glGetUniformBlockIndex(programID, "lights");

	return pointerToGPUMemory;
}

void LightsManager::BindAndConnectLights(GLuint programID, int slot)
{
	//bind the lightbuffer
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, lightBuffer);
	//connect the uniform buffer and shader
	glUniformBlockBinding(programID, lightBlockID, slot);
}