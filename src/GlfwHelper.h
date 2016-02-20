#ifndef GLFW_HELPER_H
#define GLFW_HELPER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include "GlHelper.h"

class GlfwHelper {
private:
	GLFWwindow* window;
	GlHelper* openglHelper;

public:
	GlfwHelper();
	void loop();
	GlHelper *getGlHelper();

};


#endif
