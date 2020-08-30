#ifndef __VGDE_VGDE_H__
#define __VGDE_VGDE_H__

#include "graphics/gl.h"
#include <string>
#include <vector>

#include "config.h"
#include "vec.h"
#include "graphics/videoMode.h"
#include "util/clock.h"

static const int DEFAULT_WINDOW_WIDTH  = 1024;
static const int DEFAULT_WINDOW_HEIGHT = 600;

class VGDE {
public:
	static VGDE *instance();

	int init();
	int init(VideoMode mode);
	int init(int width, int height, const std::string &title, bool fullScreen = false);

	bool running() const;
	void preRender();
	void postRender();
	void cleanUp();

	void exit();

	std::vector<VideoMode> videoModes() const;
	VideoMode videoMode() const;
	VideoMode nativeVideoMode() const;

	vec2i windowSize() const;
	void setWindowSize(const vec2i &size);

	int windowWidth() const;
	int windowHeight() const;

	std::string windowTitle() const;
	void setWindowTitle(const std::string &title);

	void windowMaximize() const;

	int fps();
	int32 frameTime() const;

	float inGameTime() const;
	float totalInGameTime() const;

private:
	friend void windowSizeCallback(GLFWwindow *, int, int);
	friend void windowCloseCallback(GLFWwindow *);

	VGDE();
	static VGDE *_instance;

    void glInit() const;
	void resize(int w, int h);

	void saveInGameTime() const;
	void loadInGameTime();

	bool _initialized;
	GLFWwindow *_window;
	int _windowWidth;
	int _windowHeight;
	std::string _windowTitle;
	bool _fullScreen;

	int _frames;
	int _frameRate;
	int _time;
	Clock _clock;
	uint32 _frameTime;
	float _startTime;
	float _totalInGameTime;
};

#endif
