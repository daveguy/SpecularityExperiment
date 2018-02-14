#ifndef CONTROLS_HPP
#define CONTROLS_HPP

void scrollFun(GLFWwindow *window, double xOffset, double yOffset);//callback function for field of view
void computeMatricesFromInputs(GLFWwindow* window);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

#endif