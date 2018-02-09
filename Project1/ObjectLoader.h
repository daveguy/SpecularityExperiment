#pragma once

#include <vector>
#include <glm/glm.hpp>

class ObjectLoader
{
public:
	bool LoadObjectFile(const char * path,
		std::vector<unsigned short> & indices,
		std::vector<glm::vec3> & vertices,
		std::vector<glm::vec2> & uvs,
		std::vector<glm::vec3> & normals);

private:

};
