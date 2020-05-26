#ifndef __VGDE_VGDE_H__
#define __VGDE_VGDE_H__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

#include "config.h"
#include "vec.h"

static const int DEFAULT_WINDOW_WIDTH  = 1024;
static const int DEFAULT_WINDOW_HEIGHT = 600;

class VGDE {
public:
	static VGDE *instance();

	int init();
	int init(int width, int height, std::string title, bool fullScreen = false);

	bool running() const;
	void preRender();
	void postRender();
	void cleanUp();

	int run();

	void exit();

	vec2i windowSize() const;
	void setWindowSize(const vec2i &size);

	int windowWidth() const;
	int windowHeight() const;

	std::string windowTitle() const;
	void setWindowTitle(const std::string &title);

	void windowMaximize() const;

private:
	friend void windowSizeCallback(GLFWwindow *, int, int);

	VGDE();
	static VGDE *_instance;

	void resize(int w, int h);
	void glInit();

	bool _initialized;
	GLFWwindow *_window;
	int _windowWidth;
	int _windowHeight;
	std::string _windowTitle;
	bool _fullScreen;
};

#endif
