/*
 * VGDE - Video Game Development Environment
 * Copyright (c) 2020 Skyler Burwell
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 */

#include "vgde.h"

#include "graphics/draw.h"
#include "input.h"
#include "util/vmath.h"

#include <fstream>
#include <iostream>

VGDE *VGDE::_instance = null;

static void windowSizeCallback(GLFWwindow *window, int w, int h) {
	VGDE *vgde = VGDE::instance();

	if (window != vgde->_window) {
		return;
	}

	vgde->resize((float)w, (float)h);
}

static void windowCloseCallback(GLFWwindow *) {
    VGDE::instance()->saveInGameTime();
}

static void glfwErrorCallback(int, const char *desc) {
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
	_time(Clock::timeAsMilliseconds()),
	_frameTime(0),
	_delta(0.f),
    _startTime(Clock::timeAsSeconds()),
    _totalInGameTime(0),
    _currentScreen(null),
    _textureSlots(0),
    _rm(null)
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
	return init((int)_windowWidth, (int)_windowHeight, _windowTitle);
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

	_windowWidth = (float)width;
	_windowHeight = (float)height;
	_windowTitle = title;
	_fullScreen = fullScreen;

	GLFWmonitor *monitor = null;
	if (_fullScreen) {
		monitor = glfwGetPrimaryMonitor();
	}

	_window = glfwCreateWindow((int)_windowWidth, (int)_windowHeight, _windowTitle.c_str(), monitor, null);

	if (!_window) {
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(_window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1); //Set to 1 to make max fps the screen refresh rate.

	glfwSetWindowSizeCallback(_window, windowSizeCallback);
	glfwSetWindowCloseCallback(_window, windowCloseCallback);

	glInit();
	drawInit();
	inputInit(_window);
	randomInit();
	_clock.restart();
	loadInGameTime();

	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &_textureSlots);
	db("OpenGL version " << glGetString(GL_VERSION));
	db("Texture slots available: " << _textureSlots);

	_rm = ResourceManager::instance();
	
	_initialized = true;
	return 0;
}

void VGDE::run() {
    if (!_initialized) {
        vgderr("Must initialize VGDE before you can run it.");
        return;
    }
    
    while (running()) {
        preRender();
        
        if (_currentScreen != null) {
            _currentScreen->render(_delta);
        }
        
        postRender();
    }
    
    cleanUp();
}

bool VGDE::running() const {
	return (!glfwWindowShouldClose(_window));
}

void VGDE::preRender() {
    _clock.restart();
	inputUpdate();

	int w, h;

	//glfwGetFramebufferSize(_window, &w, &h);
	//glViewport(0, 0, w, h);
	glClear(GL_COLOR_BUFFER_BIT);
}

void VGDE::postRender() {
    _rm->checkTimers();
    
	glfwSwapBuffers(_window);
	glfwPollEvents();

	++_frames;
	Time t = _clock.restart();
	_frameTime = t.asMilliseconds();
	_delta = t.asSeconds();
}

void VGDE::cleanUp() {
	glfwDestroyWindow(_window);
	glfwTerminate();
}

void VGDE::exit() {
    saveInGameTime();

	glfwSetWindowShouldClose(_window, GLFW_TRUE);
}

std::vector<VideoMode> VGDE::videoModes() const {
    if (!_initialized) {
        glfwInit();
    }

    std::vector<VideoMode> vModes;
    int count;
    var modes = constCast(GLFWvidmode *, glfwGetVideoModes(glfwGetPrimaryMonitor(), &count));

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

    var mode = constCast(GLFWvidmode *, glfwGetVideoMode(glfwGetPrimaryMonitor()));
    return {mode->width, mode->height, mode->refreshRate};
}

VideoMode VGDE::nativeVideoMode() const {
    return videoModes().back();
}

vec2f VGDE::windowSize() const {
	return {_windowWidth, _windowHeight};
}

void VGDE::setWindowSize(const vec2f &size) {
	glfwSetWindowSize(_window, size.x, size.y);
}

float VGDE::windowWidth() const {
	return _windowWidth;
}

float VGDE::windowHeight() const {
	return _windowHeight;
}

vec2f VGDE::windowCenter() const {
    return {_windowWidth / 2.f, _windowHeight / 2.f};
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
    if (Clock::timeAsMilliseconds() >= (_time + 1000)) {
        _time = Clock::timeAsMilliseconds();
        _frameRate = _frames;
        _frames = 0;
    }

    return _frameRate;
}

int32 VGDE::frameTime() const {
    return _frameTime;
}

float VGDE::delta() const {
    return _delta;
}

float VGDE::inGameTime() const {
    return Clock::timeAsSeconds() - _startTime;
}

float VGDE::totalInGameTime() const {
    return _totalInGameTime + inGameTime();
}

Screen *VGDE::screen() const {
    return _currentScreen;
}

void VGDE::screenAdd(Screen *screen) {
    if (screen != null) {
        bool found = false;
        
        for (var cs : _screens) {
            if (cs->name() == screen->name()) {
                found = true;
            }
        }
        
        if (!found) {
            _screens.push_back(screen);
        }
    }
}

void VGDE::screenGoto(Screen *screen, bool cleanup) {
    if (_screens.empty()) {
        vgdewarn("There are no screens to go to.");
        return;
    }
    
    if (screen == null) {
        vgdewarn("Given screen is null.");
        return;
    }
    
    var idx = std::find(_screens.begin(), _screens.end(), screen);
    if (idx == _screens.end()) {
        vgdewarn("Screen \"" << screen->name() << "\" not found.");
        return;
    }
    
    if (cleanup && _currentScreen != null){
        _currentScreen->hide();
        _rm->removeScreenTimersFor(_currentScreen);
        _screens.erase(std::find(_screens.begin(), _screens.end(), _currentScreen));
        delete _currentScreen;
        _currentScreen = null;
    } else if (_currentScreen != null) {
        _currentScreen->hide();
        _currentScreen->setActive(false);
    }
    
    _currentScreen = screen;
    _currentScreen->setActive();
    _currentScreen->show();
    _currentScreen->initialize();
    _currentScreen->resize({_windowWidth, _windowHeight});
}

void VGDE::screenGoto(const String &screen, bool cleanup) {
    if (_screens.empty()) {
        vgdewarn("There are no screens to go to.");
        return;
    }
    
    for (var sc : _screens) {
        if (sc->name() == screen) {
            screenGoto(sc, cleanup);
            return;
        }
    }
    
    vgdewarn("Screen \"" << screen << "\" not found.");
}

void VGDE::screenGoto(int index, bool cleanup) {
    if (_screens.empty()) {
        vgdewarn("There are no screens to go to.");
        return;
    }
    
    if (index >= screenCount()) {
        vgdewarn("No screen for index" << index << ".");
        return;
    }
    
    screenGoto(_screens[index], cleanup);
}

void VGDE::screenGotoFirst() {
    if (_screens.empty()) {
        vgdewarn("There are no screens to go to.");
        return;
    }
    
    screenGoto(_screens[0]);
}

void VGDE::screenGotoLast() {
    if (_screens.empty()) {
        vgdewarn("There are no screens to go to.");
        return;
    }
    
    screenGoto(_screens[screenCount() - 1]);
}

void VGDE::screenGotoNext(bool cleanup) {
    if (!screenCheckPN()) {
        return;
    }
    
    var idx = std::find(_screens.begin(), _screens.end(), _currentScreen);
    int index = std::distance(_screens.begin(), idx);
    
    if (index == screenCount() - 1) {
        vgdewarn("Already at the last screen.");
        return;
    }
    
    screenGoto(_screens[index + 1], cleanup);
}

void VGDE::screenGotoPrevious(bool cleanup) {
    if (!screenCheckPN()) {
        return;
    }
    
    var idx = std::find(_screens.begin(), _screens.end(), _currentScreen);
    int index = std::distance(_screens.begin(), idx);
    
    if (index == 0) {
        vgdewarn("Already at the first screen.");
        return;
    }
    
    screenGoto(_screens[index - 1], cleanup);
}

size_t VGDE::screenCount() const {
    return _screens.size();
}

///////////////////////////////////////////////////////////////////////////////

void VGDE::glInit() const {
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

void ::VGDE::resize(float w, float h) {
	_windowWidth = w;
	_windowHeight = h;

	glInit();

	drawSetProjection(0.0f, _windowWidth, _windowHeight, 0.0f, -1.0f, 1.0);
	
	if (_currentScreen != null) {
	    _currentScreen->resize({_windowWidth, _windowHeight});
	}
}

void VGDE::saveInGameTime() const {
    float inGameTime = 0;
    std::fstream file;

    file.open("vgdeigt", std::fstream::in);

    if (file.is_open()) {
        std::string line;
        std::getline(file, line);
        if (!line.empty()) {
            inGameTime = std::stof(line);
        }
        file.close();
    }

    file.open("vgdeigt", std::fstream::out | std::fstream::trunc);

    inGameTime += (Clock::timeAsSeconds() - _startTime);
    file << std::to_string(inGameTime);
    file.close();
}

void VGDE::loadInGameTime() {
    std::fstream file;

    file.open("vgdeigt", std::fstream::in);

    if (file.is_open()) {
        std::string line;
        std::getline(file, line);
        if (!line.empty()) {
            _totalInGameTime = std::stof(line);
        }
        file.close();
    }
}

bool VGDE::screenCheckPN() {
    if (_screens.empty()) {
        vgdewarn("There are no screens to go to.");
        return false;
    }
    
    if (screenCount() == 1) {
        vgdewarn("There is only one screen.");
        return false;
    }
    
    if (_currentScreen == null) {
        vgdewarn("No screen currently selected. Going to the first screen.");
        screenGoto(_screens[0]);
        return false;
    }
    
    return true;
}
