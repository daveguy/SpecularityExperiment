// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

#include <shader.hpp>
#include <controls.hpp>
#include "Material.h"
#include "Light.h"

int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, "Project1", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetScrollCallback(window, scrollFun);//set call back function to enable mouse scrolling

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("../Shaders/StandardShading.vert", "../Shaders/StandardShading.frag");

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "M");

	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		0.0f,  0.0f, 0.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		0.0f,  0.0f, 0.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		0.0f,  0.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		0.0f,  0.0f, 0.0f,
	};

	static const GLfloat g_normal_buffer_data[] = {
		0.0f, 0.5f, 0.5f,
		0.0f, 0.5f, 0.5f,
		0.0f, 0.5f, 0.5f,
		0.5f, 0.5, 0.0f,
		0.5f, 0.5, 0.0f,
		0.5f, 0.5, 0.0f,
		0.0f, 0.5f, -0.5f,
		0.0f, 0.5f, -0.5f,
		0.0f, 0.5f, -0.5f,
		-0.5f, 0.5, 0.0f,
		-0.5f, 0.5, 0.0f,
		-0.5f, 0.5, 0.0f,
	};

	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint normalBuffer;
	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_normal_buffer_data), g_normal_buffer_data, GL_STATIC_DRAW);

	//create a vector of lights
	std::vector<Light> lights{ 
		{ glm::vec4(4,4,4,1), glm::vec4(1,1,1,50.0) },
		{ glm::vec4(-4,4,-4,1), glm::vec4(1,1,1,50.0) }
	};

	//Create a uniform buffer
	GLuint lightBuffer;
	glGenBuffers(1, &lightBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
	glBufferData(GL_UNIFORM_BUFFER, lights.size()*sizeof(Light), &lights[0], GL_STATIC_DRAW);

	//get location in GPU memory and copy to there
	GLvoid* pointerToGPUMemory = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
	memcpy(pointerToGPUMemory, &lights[0], lights.size() * sizeof(Light));
	glUnmapBuffer(GL_UNIFORM_BUFFER);

	//Get the ID of the uniform block in the shader
	GLuint lightBlockID = glGetUniformBlockIndex(programID, "lights");


	//repeat for material a material
	Material mat1{ glm::vec4(1,0,0,0), 0.1f, 0.3f, 5.0f }; 
	GLuint materialBuffer;
	glGenBuffers(1, &materialBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, materialBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(mat1), &mat1, GL_DYNAMIC_DRAW);

	pointerToGPUMemory = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
	memcpy(pointerToGPUMemory, &mat1, sizeof(mat1));
	glUnmapBuffer(GL_UNIFORM_BUFFER);

	unsigned int materialBlockID = glGetUniformBlockIndex(programID, "material");


	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(programID);

		computeMatricesFromInputs();
		glm::mat4 projectionMatrix = getProjectionMatrix();
		glm::mat4 viewMatrix = getViewMatrix();
		glm::mat4 modelMatrix = glm::mat4(1.0); //tetrahedron is at the origin
		glm::mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);

		//Add the light
		//glUniform3f(lightId, light1.lightPosition_worldspace.x, light1.lightPosition_worldspace.y, light1.lightPosition_worldspace.z);

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

		//bind the lightbuffer
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, lightBuffer);
		//connect the uniform buffer and shader
		glUniformBlockBinding(programID, lightBlockID, 0);

		//same for material, bind to slot 1
		glBindBufferBase(GL_UNIFORM_BUFFER, 1, materialBuffer);
		glUniformBlockBinding(programID, 1, materialBlockID);

		glDrawArrays(GL_TRIANGLES, 0, 3*4);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

