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

#define TEXTURE_LOAD_ERROR 0

GLuint loadPNG(const char *filename)
{
    GLubyte *textureImage;
    bool hasAlpha;
    png_structp png_ptr;
    png_infop info_ptr;
    unsigned int sig_read = 0;
    int color_type, interlace_type;
    FILE *fp;
 
    if ((fp = fopen(filename, "rb")) == NULL)
        return 0;
 
    /* Create and initialize the png_struct
     * with the desired error handler
     * functions.  If you want to use the
     * default stderr and longjump method,
     * you can supply NULL for the last
     * three parameters.  We also supply the
     * the compiler header file version, so
     * that we know if the application
     * was compiled with a compatible version
     * of the library.  REQUIRED
     */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                     NULL, NULL, NULL);
 
    if (png_ptr == NULL) {
        fclose(fp);
        return 0;
    }
 
    /* Allocate/initialize the memory
     * for image information.  REQUIRED. */
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }
 
    /* Set error handling if you are
     * using the setjmp/longjmp method
     * (this is the normal method of
     * doing things with libpng).
     * REQUIRED unless you  set up
     * your own error handlers in
     * the png_create_read_struct()
     * earlier.
     */
    if (setjmp(png_jmpbuf(png_ptr))) {
        /* Free all of the memory associated
         * with the png_ptr and info_ptr */
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        /* If we get here, we had a
         * problem reading the file */
        return 0;
    }
 
    /* Set up the output control if
     * you are using standard C streams */
    png_init_io(png_ptr, fp);
 
    /* If we have already
     * read some of the signature */
    png_set_sig_bytes(png_ptr, sig_read);
 
    /*
     * If you have enough memory to read
     * in the entire image at once, and
     * you need to specify only
     * transforms that can be controlled
     * with one of the PNG_TRANSFORM_*
     * bits (this presently excludes
     * dithering, filling, setting
     * background, and doing gamma
     * adjustment), then you can read the
     * entire image (including pixels)
     * into the info structure with this
     * call
     *
     * PNG_TRANSFORM_STRIP_16 |
     * PNG_TRANSFORM_PACKING  forces 8 bit
     * PNG_TRANSFORM_EXPAND forces to
     *  expand a palette into RGB
     */
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);
 
    png_uint_32 width, height;
    int bit_depth;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_type, NULL, NULL);
    hasAlpha = (color_type == PNG_COLOR_TYPE_GRAY_ALPHA) || (color_type == PNG_COLOR_TYPE_RGB_ALPHA) 
                || (color_type == PNG_COLOR_MASK_ALPHA);
 
    unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    textureImage = (unsigned char*) malloc(row_bytes * height);
 
    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
 
    for (png_uint_32 i = 0; i < height; i++) {
        // note that png is ordered top to
        // bottom, but OpenGL expect it bottom to top
        // so the order or swapped
        memcpy(textureImage+(row_bytes * (height-1-i)), row_pointers[i], row_bytes);
    }
 
    /* Clean up after the read,
     * and free any memory allocated */
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
 
    /* Close the file */
    fclose(fp);

    /* Bind texture */
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, hasAlpha ? 4 : 3, width,
            height, 0, hasAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE,
            textureImage);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
    glGenerateMipmap(GL_TEXTURE_2D);

    /* That's it */
    return textureID;
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

