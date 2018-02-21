#include "Material.h"

void Material::GetUniformIDs(GLuint programID)
{
	diffuseColorID = glGetUniformLocation(programID, "diffuseColor");
	ambientPowerID = glGetUniformLocation(programID, "ambientPower");
	specularReflectanceID = glGetUniformLocation(programID, "specularReflectance");
	specularPowerID = glGetUniformLocation(programID, "specularPower");
}

void Material::SetUniforms()
{
	glUniform3f(diffuseColorID, diffuseColor.x, diffuseColor.y, diffuseColor.z);
	glUniform1f(ambientPowerID, ambientPower);
	glUniform1f(specularReflectanceID, specularReflectance);
	glUniform1f(specularPowerID, specularPower);
}
