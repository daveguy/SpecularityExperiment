// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>


// Include GLM
#include <glm/glm.hpp>

#include <shader.hpp>
#include <Controls.h>
#include "ObjectLoader.h"
#include "Material.h"
#include "LightsManager.h"
#include "Surface.h"
#include "Settings.h"

//initializes glfw and glew and creates a window
bool init(GLFWwindow** window)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return false;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	*window = glfwCreateWindow(1024, 768, "Project1", NULL, NULL);
	if (*window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(*window);
											 // Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return false;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(*window, GLFW_STICKY_KEYS, GL_TRUE);

	// Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(*window, 1024 / 2, 768 / 2);
	return true;
}

int main(void)
{
	Controls controls(settings::initialPosition, settings::initialHorizontalAngle,settings::initialVerticalAngle,
		settings::initialFoV, settings::moveSpeed, settings::scrollSpeed, settings::mouseSpeed, 0.0f);
	LightsManager lightsManager;

	GLFWwindow* window;
	if (!init(&window)) { return -1; }

	glfwSetScrollCallback(window, Controls::scrollFun);//set call back function to enable mouse scrolling

	// Dark blue background
	glClearColor(settings::backgroundColor.x, settings::backgroundColor.y, settings::backgroundColor.z, settings::backgroundColor.a);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	//create the program
	GLuint programID = glCreateProgram();

	//Load the shaders and compile them into the program
	LoadShaders(programID, "../Shaders/StandardShading.vert", "../Shaders/StandardShading.frag");

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "M");

	//create a surface and pass it the ID's it needs to set the matrix's
	Surface surface(MatrixID, ModelMatrixID, settings::pathToObject);

	GLvoid* pointerToGPUMemory = lightsManager.setUpLights(programID);

	//repeat for material a material
	Material mat1{ glm::vec4(1,0,0,0), 0.1f, 0.3f, 5.0f }; 
	GLuint materialBuffer;
	glGenBuffers(1, &materialBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, materialBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(mat1), &mat1, GL_DYNAMIC_DRAW);

	pointerToGPUMemory = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
	memcpy(pointerToGPUMemory, &mat1, sizeof(mat1));
	glUnmapBuffer(GL_UNIFORM_BUFFER);

	GLuint materialBlockID = glGetUniformBlockIndex(programID, "material");


	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(programID);

		controls.computeMatricesFromInputs(window);
		glm::mat4 projectionMatrix = controls.getProjectionMatrix();
		glm::mat4 viewMatrix = controls.getViewMatrix();

		//surface center is at the origin. 
		//Uniform scaling only! if for some reason you want to use non-uniform scaling you need to use the
		//inverse transpose model-view matrix in the fragment shader where indicated
		glm::mat4 modelMatrix = glm::mat4(1.0); 

		glm::mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;

		// Send the view matrix, the model and MVP are set by the surface
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);

		//bind lights to slot 0
		lightsManager.BindAndConnectLights(programID, 0);

		//bind material to slot 1
		glBindBufferBase(GL_UNIFORM_BUFFER, 1, materialBuffer);
		glUniformBlockBinding(programID, 1, materialBlockID);

		surface.Render(modelMatrix, viewMatrix, projectionMatrix);

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

