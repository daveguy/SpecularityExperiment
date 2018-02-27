#pragma once
#include <GL/glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>

//diffuse color, ambient power, specular reflectance, specular power
class Material
{
public:
	Material(glm::vec3 diffuseColor, float ambientPower, float specularReflectance, float specularPower)
		: diffuseColor(diffuseColor), ambientPower(ambientPower), specularReflectance(specularReflectance), specularPower(specularPower) {}

	void GetUniformIDs(GLuint programID);
	void SetUniforms();

	//all needs to be public to work with the controls easily
	glm::vec3 diffuseColor;
	float ambientPower;
	float specularReflectance;
	float specularPower;

private:
	GLuint diffuseColorID;
	GLuint ambientPowerID;
	GLuint specularReflectanceID;
	GLuint specularPowerID;
};