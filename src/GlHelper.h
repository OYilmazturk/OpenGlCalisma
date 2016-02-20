#ifndef GL_HELPER_H
#define GL_HELPER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>
#include <string.h>

#ifdef LOAD_X11
#define APIENTRY
#endif

class GlHelper {
private:
	GLuint positionBufferObject;
	GLuint colorBufferObject;
	GLuint indexBufferObject;
	GLuint theProgram;
	GLuint vao;
	GLuint offsetLocation;
	GLuint perspectiveMatrixLocation;
	float origX;
	float origY;
	float xOffset;
	float yOffset;
	float vertexPositions[8*4];
	float vertexColors[8*4];
	unsigned short vertexIndexes[8*3];
	float perspectiveMatrix[16];

public:
	enum directions { UP, LEFT, DOWN, RIGHT };
	GlHelper();
	GLuint CreateShader(GLenum, const std::string &);
	GLuint CreateProgram(const std::vector<GLuint> &);
	void InitializeProgram();
	void InitializeVertexBuffer();
	void render();
	void move(directions);
	void mmove(double, double);
};

#endif
