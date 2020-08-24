#include "vgde.h"

#include "draw.h"
#include "input.h"
#include "util/clock.h"
#include "util/vmath.h"

#include <iostream>

VGDE *VGDE::_instance = null;

static void windowSizeCallback(GLFWwindow *window, int w, int h) {
	VGDE *vgde = VGDE::instance();

	if (window != vgde->_window) {
		return;
	}

	vgde->resize(w, h);
}

static void glfwErrorCallback(int error, const char *desc) {
	std::cerr << "glfwError: " << desc << std::endl;
}

VGDE::VGDE() :
	_initialized(false),
	_window(null),
	_windowWidth(DEFAULT_WINDOW_WIDTH),
	_windowHeight(DEFAULT_WINDOW_HEIGHT),
	_windowTitle("vgde"),
	_fullScreen(false),
	_frames(0),
	_frameRate(0),
	_time(Clock::getTimeAsMilliseconds())
{
	//
}

VGDE *VGDE::instance() {
	if (_instance == null) {
		_instance = new VGDE();
	}

	return _instance;
}

int VGDE::init() {
	return init(_windowWidth, _windowHeight, _windowTitle);
}

int VGDE::init(VideoMode mode) {
    return init(mode.width, mode.height, "", true);
}

int VGDE::init(int width, int height, const std::string &title, bool fullScreen) {
	if (_initialized) {
		vgdewarn("VGDE already initialized!");
		return 0;
	}

	glfwSetErrorCallback(glfwErrorCallback);

	if (!glfwInit()) {
		return 1;
	}

	_windowWidth = width;
	_windowHeight = height;
	_windowTitle = title;
	_fullScreen = fullScreen;

	GLFWmonitor *monitor = null;
	if (_fullScreen) {
		monitor = glfwGetPrimaryMonitor();
	}

	_window = glfwCreateWindow(_windowWidth, _windowHeight, _windowTitle.c_str(), monitor, null);

	if (!_window) {
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(_window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	glfwSetWindowSizeCallback(_window, windowSizeCallback);

	glInit();
	drawInit();
	inputInit(_window);
	randomInit();

	db("OpenGL version " << glGetString(GL_VERSION));

	_initialized = true;
	return 0;
}

bool VGDE::running() const {
	return (!glfwWindowShouldClose(_window));
}

void VGDE::preRender() {
	inputUpdate();

	int w, h;

	glfwGetFramebufferSize(_window, &w, &h);
	glViewport(0, 0, w, h);
	glClear(GL_COLOR_BUFFER_BIT);
}

void VGDE::postRender() {
	glfwSwapBuffers(_window);
	glfwPollEvents();

	++_frames;
}

void VGDE::cleanUp() {
	glfwDestroyWindow(_window);
	glfwTerminate();
}

void VGDE::exit() {
	glfwSetWindowShouldClose(_window, GLFW_TRUE);
}

std::vector<VideoMode> VGDE::videoModes() const {
    if (!_initialized) {
        glfwInit();
    }

    std::vector<VideoMode> vModes;
    int count;
    GLFWvidmode *modes = constCast(GLFWvidmode *, glfwGetVideoModes(glfwGetPrimaryMonitor(), &count));

    vModes.reserve(count);
    for (int i = 0; i < count; ++i) {
        vModes.push_back({modes[i].width, modes[i].height, modes[i].refreshRate});
    }

    return vModes;
}

VideoMode VGDE::videoMode() const {
    if (!_initialized) {
        return {0};
    }

    GLFWvidmode *mode = constCast(GLFWvidmode *, glfwGetVideoMode(glfwGetPrimaryMonitor()));
    return {mode->width, mode->height, mode->refreshRate};
}

VideoMode VGDE::nativeVideoMode() const {
    return videoModes().back();
}

vec2i VGDE::windowSize() const {
	return {_windowWidth, _windowHeight};
}

void VGDE::setWindowSize(const vec2i &size) {
	glfwSetWindowSize(_window, size.x, size.y);
}

int VGDE::windowWidth() const {
	return _windowWidth;
}

int VGDE::windowHeight() const {
	return _windowHeight;
}

std::string VGDE::windowTitle() const {
	return _windowTitle;
}

void VGDE::setWindowTitle(const std::string &title) {
	glfwSetWindowTitle(_window, title.c_str());
}

void VGDE::windowMaximize() const {
	glfwMaximizeWindow(_window);
}

int VGDE::fps() {
    if (Clock::getTimeAsMilliseconds() >= (_time + 1000)) {
        _time = Clock::getTimeAsMilliseconds();
        _frameRate = _frames;
        _frames = 0;
    }

    return _frameRate;
}

///////////////////////////////////////////////////////////////////////////////

void VGDE::glInit() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_TEXTURE_2D);
	glViewport(0, 0, _windowWidth, _windowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, _windowWidth, _windowHeight, 0.f, -1.f, 1.f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
}

void ::VGDE::resize(int w, int h) {
	_windowWidth = w;
	_windowHeight = h;

	glInit();

	drawSetProjection(0.0f, (float)_windowWidth, (float)_windowHeight, 0.0f, -1.0f, 1.0);
}