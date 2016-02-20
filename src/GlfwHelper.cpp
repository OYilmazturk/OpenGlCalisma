#include "GlfwHelper.h"

GlHelper * GlfwHelper::getGlHelper() {
	return openglHelper;
}

static void mouse_callback(GLFWwindow* window, double xoffset, double yoffset) {

	GlfwHelper *glfwHelper = static_cast<GlfwHelper*>(glfwGetWindowUserPointer(window));
	glfwHelper->getGlHelper()->mmove(xoffset, yoffset);
	
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	GlfwHelper *glfwHelper = static_cast<GlfwHelper*>(glfwGetWindowUserPointer(window));

	if (action == GLFW_PRESS) {
		switch(key) {
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, GL_TRUE);
				break;
			case GLFW_KEY_UP:
				glfwHelper->getGlHelper()->move(GlHelper::UP);
				break;
			case GLFW_KEY_DOWN:
				glfwHelper->getGlHelper()->move(GlHelper::DOWN);
				break;
			case GLFW_KEY_LEFT:
				glfwHelper->getGlHelper()->move(GlHelper::LEFT);
				break;
			case GLFW_KEY_RIGHT:
				glfwHelper->getGlHelper()->move(GlHelper::RIGHT);
				break;
		}
	}
}

void GlfwHelper::loop() {
	do {
		openglHelper->render();
		glfwSwapBuffers(window);
		glfwPollEvents();
	} while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
}

GlfwHelper::GlfwHelper() {
	if (!glfwInit()) {
		std::cerr << "glfw baslatilamadi" << std::endl;
		return;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	this->window = glfwCreateWindow(1024, 768, "AB2016-3D-Ornek-1", NULL, NULL);

	if (window == NULL) {
		std::cerr << "GLFW penceresi acilamadi" << std::endl;
		glfwTerminate();
		return;
	}

	glfwSetWindowUserPointer(window, this);

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		std::cerr << "GLEW baslatilamadi" << std::endl;
		return;
	}


	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	openglHelper = new GlHelper();
	if (openglHelper == NULL) {
		glfwTerminate();
		std::cerr << "GlHelper baslatilamadi" << std::endl;
		return;
	}

}
