#ifndef VBOINDEXER_HPP
#define VBOINDEXER_HPP

#include "../config.hh"

void indexVBO(
	std::vector<vec3data> & in_vertices,
	std::vector<vec2data> & in_uvs,
	std::vector<vec3data> & in_normals,

	std::vector<index_type> & out_indices,
	std::vector<vec3data> & out_vertices,
	std::vector<vec2data> & out_uvs,
	std::vector<vec3data> & out_normals
);


void indexVBO_TBN(
	std::vector<glm::vec3> & in_vertices,
	std::vector<glm::vec2> & in_uvs,
	std::vector<glm::vec3> & in_normals,
	std::vector<glm::vec3> & in_tangents,
	std::vector<glm::vec3> & in_bitangents,

	std::vector<unsigned short> & out_indices,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals,
	std::vector<glm::vec3> & out_tangents,
	std::vector<glm::vec3> & out_bitangents
);

#endif
