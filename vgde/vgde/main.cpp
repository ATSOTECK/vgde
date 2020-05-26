#include "config.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "draw.h"

int main() {
	if (!glfwInit()) {
		return 1;
	}

	GLFWwindow *window = glfwCreateWindow(640, 480, "title", null, null);

	if (!window) {
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	//glViewport(0, 0, width, height);


	while (!glfwWindowShouldClose(window)) {
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
