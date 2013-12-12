#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include <GL/glew.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

//////////////////////////////////////////////////////////////////////////////////////
////// Load DDS texture
//////////////////////////////////////////////////////////////////////////////////////

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

GLuint loadDDS(const char * imagepath){

	unsigned char header[124];

	FILE *fp; 
 
	/* try to open the file */ 
	fp = fopen(imagepath, "rb"); 
	if (fp == NULL){
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar(); 
		return 0;
	}
   
	/* verify the type of file */ 
	char filecode[4]; 
	fread(filecode, 1, 4, fp); 
	if (strncmp(filecode, "DDS ", 4) != 0) { 
		fclose(fp); 
		return 0; 
	}
	
	/* get the surface desc */ 
	fread(&header, 124, 1, fp); 

	unsigned int height      = *reinterpret_cast<unsigned int*>(header +  8); 
	unsigned int width	 = *reinterpret_cast<unsigned int*>(header + 12);
	unsigned int linearSize	 = *reinterpret_cast<unsigned int*>(header + 16);
	unsigned int mipMapCount = *reinterpret_cast<unsigned int*>(header + 24);
	unsigned int fourCC      = *reinterpret_cast<unsigned int*>(header + 80);

 
	unsigned char * buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */ 
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize; 
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char)); 
	fread(buffer, 1, bufsize, fp); 
	/* close the file pointer */ 
	fclose(fp);

//	unsigned int components  = (fourCC == FOURCC_DXT1) ? 3 : 4; 
	unsigned int format;
	unsigned int blockSize;
	switch(fourCC) 
	{ 
	case FOURCC_DXT1: 
#ifdef GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; 
                blockSize = 8;
#else
                fprintf(stderr, "Not support GL_COMPRESSED_RGBA_S3TC_DXT1_EXT\n");
                return 0;
#endif
		break; 
	case FOURCC_DXT3: 
#ifdef GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; 
                blockSize = 16;
#else
                fprintf(stderr, "Not support GL_COMPRESSED_RGBA_S3TC_DXT3_EXT\n");
                return 0;
#endif
		break; 
	case FOURCC_DXT5: 
#ifdef GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; 
                blockSize = 16;
#else
                fprintf(stderr, "Not support GL_COMPRESSED_RGBA_S3TC_DXT5_EXT\n");
                return 0;
#endif
		break; 
	default: 
		free(buffer); 
		return 0; 
	}

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);	
	
	unsigned int offset = 0;

	/* load the mipmaps */ 
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level) 
	{ 
		unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize; 
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,  
			0, size, buffer + offset); 
	 
		offset += size; 
		width  /= 2; 
		height /= 2; 

		// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
		if(width < 1) width = 1;
		if(height < 1) height = 1;

	}

	free(buffer); 

	return textureID;


}

//////////////////////////////////////////////////////////////////////////////////////
////// Load PNG texture
//////////////////////////////////////////////////////////////////////////////////////

#include <GLES2/gl2.h>
#include <png.h>
#include <cstdio>
#include <string>
#include <cstring>

// Modified from:
// http://stackoverflow.com/questions/11296644/loading-png-textures-to-opengl-with-libpng-only

GLuint loadPNG(const char *file_name)
{
    png_byte header[8];

    FILE *fp = fopen(file_name, "rb");
    if (fp == 0)
    {
        perror(file_name);
        return 0;
    }

    // read the header
    fread(header, 1, 8, fp);

    if (png_sig_cmp(header, 0, 8))
    {
        fprintf(stderr, "error: %s is not a PNG.\n", file_name);
        fclose(fp);
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
        fprintf(stderr, "error: png_create_read_struct returned 0.\n");
        fclose(fp);
        return 0;
    }

    // create png info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        fprintf(stderr, "error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        fclose(fp);
        return 0;
    }

    // create png info struct
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info)
    {
        fprintf(stderr, "error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
        fclose(fp);
        return 0;
    }

    // the code in this if statement gets called if libpng encounters an error
    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "error from libpng\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return 0;
    }

    // init png reading
    png_init_io(png_ptr, fp);

    // let libpng know you already read the first 8 bytes
    png_set_sig_bytes(png_ptr, 8);

    // read all the info up to the image data
    png_read_info(png_ptr, info_ptr);

    // variables to pass to get info
    int bit_depth, color_type;
    png_uint_32 temp_width, temp_height;

    // get info about png
    png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth, &color_type,
        NULL, NULL, NULL);

    // Update the png info struct.
    png_read_update_info(png_ptr, info_ptr);

    // Row size in bytes.
    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

    // glTexImage2d requires rows to be 4-byte aligned
    rowbytes += 3 - ((rowbytes-1) % 4);

    // Allocate the image_data as a big block, to be given to opengl
    png_byte * image_data;
    image_data = (png_byte*) malloc(rowbytes * temp_height * sizeof(png_byte)+15);
    if (image_data == NULL)
    {
        fprintf(stderr, "error: could not allocate memory for PNG image data\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return 0;
    }

    // row_pointers is for pointing to image_data for reading the png with libpng
    png_bytep * row_pointers = (png_bytep*) malloc(temp_height * sizeof(png_bytep));
    if (row_pointers == NULL)
    {
        fprintf(stderr, "error: could not allocate memory for PNG row pointers\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        free(image_data);
        fclose(fp);
        return 0;
    }

    // set the individual row_pointers to point at the correct offsets of image_data
    png_uint_32 i;
    for (i = 0; i < temp_height; i++)
    {
        row_pointers[temp_height - 1 - i] = image_data + i * rowbytes;
    }

    // read the png into image_data through row_pointers
    png_read_image(png_ptr, row_pointers);

    // format
    GLint format = 0, internalformat = 0;
    switch(color_type) {
        case PNG_COLOR_TYPE_GRAY:
            format = internalformat = GL_LUMINANCE;
            break;
        case PNG_COLOR_TYPE_GRAY_ALPHA:
            format = internalformat = GL_LUMINANCE_ALPHA;
            break;
        case PNG_COLOR_TYPE_RGB:
            format = internalformat = GL_RGB;
            break;
        case PNG_COLOR_TYPE_RGB_ALPHA:
            format = internalformat = GL_RGBA;
            break;
    }
    if (color_type & PNG_COLOR_MASK_PALETTE)
        format = internalformat = 0;


    // Generate the OpenGL texture object
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, format, temp_width, temp_height, 0, internalformat, GL_UNSIGNED_BYTE, image_data);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);

    // clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    free(image_data);
    free(row_pointers);
    fclose(fp);
    return texture;
}


GLuint loadTexture(char const *prefix) {
    static bool support_dxt1 = std::strstr(reinterpret_cast<char const*>(glGetString(GL_EXTENSIONS)), 
                                           "GL_EXT_texture_compression_dxt1");
    std::string filename = prefix;
    if (support_dxt1) {
        filename += ".dds";
        std::fprintf(stderr, "Loading texture %s\n", filename.c_str());
        return loadDDS(filename.c_str());
    } else {
        filename += ".png";
        std::fprintf(stderr, "Loading texture %s\n", filename.c_str());
        return loadPNG(filename.c_str());
    }
    return 0;
}
