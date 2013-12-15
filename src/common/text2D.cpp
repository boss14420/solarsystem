#include <vector>
#include <cstring>

#include <GLES2/gl2.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "../config.hh"
#include "../graphics/shader.hh"
#include "texture.hpp"

#include "text2D.hpp"

unsigned int Text2DTextureID;              // Texture containing the font
unsigned int Text2DVertexBufferID;         // Buffer containing the vertices
unsigned int Text2DUVBufferID;             //                       UVs
unsigned int Text2DShaderID;               // Program used to disaply the text
unsigned int vertexPosition_screenspaceID; // Location of the program's "vertexPosition_screenspace" attribute
unsigned int vertexUVID;                   // Location of the program's "vertexUV" attribute
unsigned int Text2DUniformID;              // Location of the program's texture attribute
unsigned int windowSizeID;                 // Location of the program's "window_size" attribute
ShaderProgram text2D_shader;

void initText2D(const char * texturePath){

	// Initialize texture
	Text2DTextureID = loadPNG(texturePath);

	// Initialize VBO
	glGenBuffers(1, &Text2DVertexBufferID);
	glGenBuffers(1, &Text2DUVBufferID);

	// Initialize Shader
        text2D_shader.link("shaders/TextVertexShader.vertexshader", "shaders/TextVertexShader.fragmentshader");

	// Get a handle for our buffers
        vertexPosition_screenspaceID = text2D_shader.get_attrib_location("vertexPosition_screenspace");
        vertexUVID = text2D_shader.get_attrib_location("vertexUV");

	// Initialize uniforms' IDs
        Text2DUniformID = text2D_shader.get_uniform_location("myTextureSampler");
        windowSizeID = text2D_shader.get_uniform_location("window_size");
}

void printText2D(const char * text, int x, int y, int size, int window_w, int window_h) {

	unsigned int length = strlen(text);

	// Fill buffers
	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> UVs;
        vertices.reserve(6*length);
        UVs.reserve(6*length);
	for ( unsigned int i=0 ; i<length ; i++ ){
		
		glm::vec2 vertex_up_left    = glm::vec2( x+i*size     , y+size );
		glm::vec2 vertex_up_right   = glm::vec2( x+i*size+size, y+size );
		glm::vec2 vertex_down_right = glm::vec2( x+i*size+size, y      );
		glm::vec2 vertex_down_left  = glm::vec2( x+i*size     , y      );

		vertices.push_back(vertex_up_left   );
		vertices.push_back(vertex_down_left );
		vertices.push_back(vertex_up_right  );

		vertices.push_back(vertex_down_right);
		vertices.push_back(vertex_up_right);
		vertices.push_back(vertex_down_left);

		char character = text[i];
		float uv_x = (character%16)/16.0f;
		float uv_y = (character/16)/16.0f;

		glm::vec2 uv_up_left    = glm::vec2( uv_x           , 1.0f - uv_y );
		glm::vec2 uv_up_right   = glm::vec2( uv_x+1.0f/16.0f, 1.0f - uv_y );
		glm::vec2 uv_down_right = glm::vec2( uv_x+1.0f/16.0f, 1.0f - (uv_y + 1.0f/16.0f) );
		glm::vec2 uv_down_left  = glm::vec2( uv_x           , 1.0f - (uv_y + 1.0f/16.0f) );
		UVs.push_back(uv_up_left   );
		UVs.push_back(uv_down_left );
		UVs.push_back(uv_up_right  );

		UVs.push_back(uv_down_right);
		UVs.push_back(uv_up_right);
		UVs.push_back(uv_down_left);
	}
	glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);

	// Bind shader
//	glUseProgram(Text2DShaderID);
        text2D_shader.use();

	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Text2DTextureID);
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(Text2DUniformID, 0);

        // Bind window size
        glUniform2f(windowSizeID, (float)window_w, (float)window_h); 

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(vertexPosition_screenspaceID);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
	glVertexAttribPointer(vertexPosition_screenspaceID, 2, FLOAT_TYPE, GL_FALSE, 0, (void*)0 );

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(vertexUVID);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
	glVertexAttribPointer(vertexUVID, 2, FLOAT_TYPE, GL_FALSE, 0, (void*)0 );

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw call
	glDrawArrays(GL_TRIANGLES, 0, vertices.size() );

	glDisable(GL_BLEND);

	glDisableVertexAttribArray(vertexPosition_screenspaceID);
	glDisableVertexAttribArray(vertexUVID);

}

void cleanupText2D(){

	// Delete buffers
	glDeleteBuffers(1, &Text2DVertexBufferID);
	glDeleteBuffers(1, &Text2DUVBufferID);

	// Delete texture
	glDeleteTextures(1, &Text2DTextureID);

//	// Delete shader
//	glDeleteProgram(Text2DShaderID);
}
