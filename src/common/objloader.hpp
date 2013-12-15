#ifndef OBJLOADER_H
#define OBJLOADER_H

#include "../config.hh"

bool loadOBJ(
	const char * path, 
	std::vector<vec3data> & out_vertices, 
	std::vector<vec2data> & out_uvs, 
	std::vector<vec3data> & out_normals
);



bool loadAssImp(
	const char * path, 
	std::vector<unsigned short> & indices,
	std::vector<glm::vec3> & vertices,
	std::vector<glm::vec2> & uvs,
	std::vector<glm::vec3> & normals
);

#endif
