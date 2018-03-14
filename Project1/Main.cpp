// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>

#include "AntTweakBar.h"

// Include GLM
#include <glm/glm.hpp>

#include <shader.hpp>
#include <Controls.h>
#include "ObjectLoader.h"
#include "Material.h"
#include "LightsManager.h"
#include "Surface.h"
#include "Settings.h"
#include <iostream>

//variables outside main so callback functions can access them
Controls controls(settings::initialPosition, settings::initialHorizontalAngle, settings::initialVerticalAngle,
	settings::initialFoV, settings::moveSpeed, settings::scrollSpeed, settings::mouseSpeed, 0.0f, false);

bool showGUI = false;
bool fullScreen = false;
glm::vec2 monitorResolution;
glm::vec2 currentResolution(settings::winDimemsions.x, settings::winDimemsions.y);
float interocularDistance = settings::defaultInterocularDistance;

enum SpecularityType
{
	GLOSSY,
	STATIC,
	PARALLEL,
};
SpecularityType specularityType(GLOSSY);


//TODO add static and parallel specularities

void GetResolution(glm::vec2& resolution)
{
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	resolution.x = mode->width;
	resolution.y = mode->height;
}

static void glfwError(int id, const char* description)
{
	std::cout << description << std::endl;
}

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

	glfwWindowHint(GLFW_STEREO, GLFW_TRUE);

	// Open a window and create its OpenGL context
	*window = glfwCreateWindow(settings::winDimemsions.x, settings::winDimemsions.y, "Project1", NULL, NULL);
	if (*window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible.\n");
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
	glfwSetCursorPos(*window, settings::winDimemsions.x / 2, settings::winDimemsions.y / 2);
	return true;
}

//TODO set up GUI to display/change the lights?
void initGUI(GLFWwindow* window, Controls& controls, Material& material)
{
	TwInit(TW_OPENGL_CORE, NULL);
	TwWindowSize(settings::winDimemsions.x, settings::winDimemsions.y);
	TwDefine(" TW_HELP visible=false ");

	TwBar* positionGUI = TwNewBar("Camera Position");
	TwSetParam(positionGUI, NULL, "refresh", TW_PARAM_CSTRING, 1, "0.1");
	TwSetParam(positionGUI, NULL, "size", TW_PARAM_CSTRING, 1, "200 100");
	TwAddVarRW(positionGUI, "Pos X", TW_TYPE_FLOAT, &controls.position.x, "step=0.01");
	TwAddVarRW(positionGUI, "Pos Y", TW_TYPE_FLOAT, &controls.position.y, "step=0.01");
	TwAddVarRW(positionGUI, "Pos Z", TW_TYPE_FLOAT, &controls.position.z, "step=0.01");

	TwBar* materialGUI = TwNewBar("Material");
	TwSetParam(materialGUI, NULL, "position", TW_PARAM_CSTRING, 1, "16 125");
	TwSetParam(materialGUI, NULL, "size", TW_PARAM_CSTRING, 1, "200 150");
	TwAddVarRW(materialGUI, "R", TW_TYPE_FLOAT, &material.diffuseColor.x, "min=0, max=1, step=0.01");
	TwAddVarRW(materialGUI, "G", TW_TYPE_FLOAT, &material.diffuseColor.y, "min=0, max=1, step=0.01");
	TwAddVarRW(materialGUI, "B", TW_TYPE_FLOAT, &material.diffuseColor.z, "min=0, max=1, step=0.01");
	TwAddVarRW(materialGUI, "Ambient Power", TW_TYPE_FLOAT, &material.ambientPower, "min=0, max=1, step=0.01");
	TwAddVarRW(materialGUI, "Specular Reflectance", TW_TYPE_FLOAT, &material.specularReflectance, "min=0, max=5, step=0.01");
	TwAddVarRW(materialGUI, "Specular Power", TW_TYPE_FLOAT, &material.specularPower, "min=0");

	TwBar* interocularDistanceGUI = TwNewBar("Interocular Distance");
	TwSetParam(interocularDistanceGUI, NULL, "position", TW_PARAM_CSTRING, 1, "16 300");
	TwSetParam(interocularDistanceGUI, NULL, "size", TW_PARAM_CSTRING, 1, "200 25");
	TwAddVarRW(interocularDistanceGUI, "distance", TW_TYPE_FLOAT, &interocularDistance, "min=0, max=1, step=0.01");
}

void toggleGUI(GLFWwindow* window)
{
	showGUI = !showGUI;

	if (showGUI)
	{
		controls.EnableMouse();
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwSetMouseButtonCallback(window, (GLFWmousebuttonfun)TwEventMouseButtonGLFW3);
		glfwSetCursorPosCallback(window, (GLFWcursorposfun)TwEventMousePosGLFW3);
	}
	else
	{
		// Reset mouse position for next frame otherwise it moves the scene
		glfwSetCursorPos(window, currentResolution.x / 2, currentResolution.y / 2);
		controls.DisableMouse();
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetMouseButtonCallback(window, NULL);
		glfwSetCursorPosCallback(window, NULL);
	}
}

void toggleFullScreen(GLFWwindow* window)
{
	fullScreen = !fullScreen;
	if (fullScreen)
	{
		currentResolution = glm::vec2(monitorResolution.x, monitorResolution.y);
		glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, currentResolution.x, currentResolution.y, GLFW_DONT_CARE);
		TwWindowSize(currentResolution.x, currentResolution.y);

	}
	else
	{
		currentResolution = glm::vec2(settings::winDimemsions.x, settings::winDimemsions.y);
		glfwSetWindowMonitor(window, NULL, 100, 100, currentResolution.x, currentResolution.y, GLFW_DONT_CARE);
		TwWindowSize(currentResolution.x, currentResolution.y);
		glfwSetCursorPos(window, currentResolution.x / 2, currentResolution.y / 2);
	}
	int VPx;
	int VPy;
	glfwGetFramebufferSize(window, &VPx, &VPy);
	glViewport(0, 0, VPx, VPy);
}

void toggleSpecularityType(int key)
{
	if (key == GLFW_KEY_1)
	{
		specularityType = GLOSSY;
	}
	else if (key == GLFW_KEY_2)
	{
		specularityType = STATIC;
	}
	else
	{
		specularityType = PARALLEL;
	}
}

//callback function to toggle GUI on and off and enable mouse, and to enable fullscreen
void keyPressCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_G && action == GLFW_PRESS)
	{
		toggleGUI(window);
	}
	else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		toggleFullScreen(window);
	}
	else if ((key == GLFW_KEY_1 || key == GLFW_KEY_2 || key == GLFW_KEY_3) && action == GLFW_PRESS)
	{
		toggleSpecularityType(key);
	}
}

void setSpecularityCameraPosition(const Controls &controls, glm::vec3 &specularityCameraPosition)
{
	if (specularityType == GLOSSY)
	{
		specularityCameraPosition = glm::vec3(controls.position);
	}
	else if (specularityType == STATIC)
	{
		specularityCameraPosition = glm::vec3(settings::specularityPositionStatic);
	}
	else
	{
		specularityCameraPosition = glm::vec3(settings::specularityPositionParallel);
	}
}

int main(void)
{
	
	LightsManager lightsManager;

	glfwSetErrorCallback(&glfwError);
	GLFWwindow* window;
	if (!init(&window)) { return -1; }

	GetResolution(monitorResolution);

	glfwSetScrollCallback(window, Controls::scrollFun);//set call back function to enable mouse scrolling
	glfwSetKeyCallback(window, keyPressCallback);

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

	//repeat for a material
	Material mat1( glm::vec3(0.0f,0.0f,0.0f), 0.0f, 0.8f, 150.0f ); 
	mat1.GetUniformIDs(programID);

	//set up the GUI
	initGUI(window, controls, mat1);
	//Used to calculate the specular component in the fragment shader, may or may not correspoond to the actual camera location
	glm::vec3 specularityCameraPosition;
	GLuint specularityCameraPositionID = glGetUniformLocation(programID, "cameraPosition_worldspace");

	//surface center is at the origin. 
	//Uniform scaling only! if for some reason you want to use non-uniform scaling you need to use the
	//inverse transpose model-view matrix in the fragment shader where indicated
	glm::mat4 modelMatrix = glm::mat4(1.0); 
	modelMatrix = glm::rotate(modelMatrix, 7 * 3.14f/180, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, -2.0f));
	do {
		

		glUseProgram(programID);

		controls.computeMatricesFromInputs(window, currentResolution);
		glm::mat4 projectionMatrix = controls.getProjectionMatrix();
		glm::mat4 viewMatrix = controls.getViewMatrix();

		glm::mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;

		//bind lights to slot 0
		lightsManager.BindAndConnectLights(programID, 0);

		//set material uniforms
		mat1.SetUniforms();

		//update camera postion for calculating specularities
		setSpecularityCameraPosition(controls, specularityCameraPosition);
		glUniform3f(specularityCameraPositionID, specularityCameraPosition.x, specularityCameraPosition.y, specularityCameraPosition.z);

		//3D does not work windowed
		if (!fullScreen)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			// Send the view matrix, the model and MVP are set by the surface
			glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);
			if (showGUI)
			{
				TwDraw();
			}
			surface.Render(modelMatrix, viewMatrix, projectionMatrix);
		}
		else
		{
			glDrawBuffer(GL_BACK_LEFT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			// Send the view matrix, the model and MVP are set by the surface
			glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);

			if (showGUI)
			{
				TwDraw();
			}
			surface.Render(modelMatrix, viewMatrix, projectionMatrix);

			glDrawBuffer(GL_BACK_RIGHT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			viewMatrix = glm::translate(viewMatrix, glm::vec3(interocularDistance, 0, 0));
			glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);

			surface.Render(modelMatrix, viewMatrix, projectionMatrix);

			if (showGUI)
			{
				TwDraw();
			}
		}

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Close GUI and OpenGL window and terminate GLFW
	TwTerminate();
	glfwTerminate();
	return 0;
}

