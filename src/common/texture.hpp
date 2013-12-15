#ifndef TEXTURE_HPP
#define TEXTURE_HPP

GLuint loadDDS(const char * imagepath);

GLuint loadPNG(const char *file_name);

GLuint loadTexture(char const *prefix);

#endif
