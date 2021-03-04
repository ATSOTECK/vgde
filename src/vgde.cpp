/*
 * VGDE - Video Game Development Environment
 * Copyright (c) 2020-2021 Skyler Burwell
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

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <thread>

#define VSYNC_ON 1
#define VSYNC_OFF 0

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

static void windowFocusCallback(GLFWwindow *window, int focus) {
    VGDE *vgde = VGDE::instance();
    
    if (window != vgde->_window || !vgde->pauseIfNotFocused() || vgde->screen() == null) {
        return;
    }
    
    if (focus) {
        vgde->screen()->resume();
    } else {
        vgde->screen()->pause();
    }
}

static void glfwErrorCallback(int, const char *desc) {
	std::cerr << "glfwError: " << desc << std::endl;
}

VGDE::VGDE() :
	_initialized(false),
	_window(null),
	_vsync(true),
	_windowWidth(DEFAULT_WINDOW_WIDTH),
	_windowHeight(DEFAULT_WINDOW_HEIGHT),
	_windowTitle("vgde"),
	_fullScreen(false),
	_pauseINF(false),
	_frames(0),
	_frameRate(0),
	_time(Clock::timeAsMilliseconds()),
	_frameTime(0),
	_delta(0.f),
    _startTime(Clock::timeAsSeconds()),
    _totalInGameTime(0),
    _currentScreen(null),
    _ssname("screenshot"),
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
		vWarn("VGDE is already initialized!");
		return 0;
	}
    
    var consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
 
	_consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	_fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("log.txt", true);
	
    _vgdeLogger = std::make_shared<spdlog::logger>(spdlog::logger("vgde", {consoleSink, _fileSink}));
    _logger = std::make_shared<spdlog::logger>(spdlog::logger("game", {_consoleSink, _fileSink}));
    _loggerDebug = false;
    
    _loggerPattern = "[%I:%M:%S %p] [%n] [%^%l%$] %v";
	spdlog::set_default_logger(_logger);
    spdlog::set_pattern(_loggerPattern.stdString());
    _vgdeLogger->set_pattern(_loggerPattern.stdString());
#ifdef VDEBUG
    showVGDEDebugMessages();
    showDebugMessages();
#endif

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
	glfwSwapInterval(VSYNC_ON);

	glfwSetWindowSizeCallback(_window, windowSizeCallback);
	glfwSetWindowCloseCallback(_window, windowCloseCallback);
	glfwSetWindowFocusCallback(_window, windowFocusCallback);
	
	glInit();
	drawInit();
	inputInit(_window);
	randomInit();
	_clock.restart();
	loadInGameTime();

	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &_textureSlots);
	vDebug("OpenGL version {}", glGetString(GL_VERSION));
	vDebug("Texture slots available: {}", _textureSlots);
    
    stbi_write_png_compression_level = 0; //Try to speed up saving screenshots.

	_rm = ResourceManager::instance();
	
	_initialized = true;
	return 0;
}

void VGDE::run() {
    if (!_initialized) {
        vError("Must initialize VGDE before you can run it.");
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
	
    glClear(GL_COLOR_BUFFER_BIT);
}

void VGDE::postRender() {
    _rm->checkTimers();
    
	glfwSwapBuffers(_window);
	glfwPollEvents();

	++_frames;
    _frameTime = _clock.restart().asMilliseconds();
	_delta = _frameTime / 1000.f; //Convert frame time to seconds.
    
    if (Clock::timeAsMilliseconds() >= (_time + 1000)) {
        _time = Clock::timeAsMilliseconds();
        _frameRate = _frames;
        _frames = 0;
    }
}

void VGDE::cleanUp() {
    _rm->cleanUp();
    drawCleanUp();
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

bool VGDE::windowVSync() const {
    return _vsync;
}

void VGDE::windowSetVSync(bool vsync) {
    if (_vsync == vsync) {
        return;
    }
    
    _vsync = vsync;
    
    if (_vsync) {
        glfwSwapInterval(VSYNC_ON);
    } else {
        glfwSwapInterval(VSYNC_OFF);
    }
}

vec2f VGDE::windowSize() const {
	return {_windowWidth, _windowHeight};
}

void VGDE::windowSetSize(const vec2f &size) {
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

void VGDE::windowSetTitle(const std::string &title) {
	glfwSetWindowTitle(_window, title.c_str());
}

void VGDE::windowMaximize() const {
	glfwMaximizeWindow(_window);
}

bool VGDE::windowFocused() const {
    return glfwGetWindowAttrib(_window, GLFW_FOCUSED);
}

bool VGDE::pauseIfNotFocused() const {
    return _pauseINF;
}

void VGDE::pauseIfNotFocused(bool pause) {
    _pauseINF = pause;
}

int VGDE::fps() const {
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

void VGDE::screenshotSetName(const String &name) {
    if (!name.empty()) {
        _ssname = name;
    }
}

void VGDE::screenshot() {
    String name = _ssname + ".png";
    bool exists = true;
    int count = 1;
    
    while (exists) {
        if (std::filesystem::exists(name.c_str())) {
            name = _ssname + "_" + String(count++) + ".png";
        } else {
            exists = false;
        }
    }
    
    int w = (int)_windowWidth;
    int h = (int)_windowHeight;
    GLsizei size = w * h * 4u;
    var data = new uint8[size];
    var pixels = new uint8[size];
    
    glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    for (int y = 0; y < h; ++y) {
        uint8 *ptr = &data[(h - y - 1) * w * 4];
        memcpy_s(&pixels[y * w * 4], w * 4, ptr, w * 4);
    }
    
    delete[] data;
    
    //TODO(Skyler): Is there anyway to make this faster?
    std::thread([=]{
        stbi_write_png(name.c_str(), w, h, 4, pixels, 0);
    
        delete[] pixels;
    }).detach();
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
        vWarn("There are no screens to go to.");
        return;
    }
    
    if (screen == null) {
        vWarn("Given screen is null.");
        return;
    }
    
    var idx = std::find(_screens.begin(), _screens.end(), screen);
    if (idx == _screens.end()) {
        vWarn("Screen \"{}\" not found.", screen->name());
        return;
    }
    
    if (cleanup && _currentScreen != null){
        _currentScreen->hide();
        _rm->removeScreenTimersFor(_currentScreen);
        _rm->removeLambdaTimers();
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
        vWarn("There are no screens to go to.");
        return;
    }
    
    for (var sc : _screens) {
        if (sc->name() == screen) {
            screenGoto(sc, cleanup);
            return;
        }
    }
    
    vWarn("Screen \"{}\" not found.", screen);
}

void VGDE::screenGoto(int index, bool cleanup) {
    if (_screens.empty()) {
        vWarn("There are no screens to go to.");
        return;
    }
    
    if (index >= screenCount()) {
        vWarn("No screen for index {}.", index);
        return;
    }
    
    screenGoto(_screens[index], cleanup);
}

void VGDE::screenGotoFirst() {
    if (_screens.empty()) {
        vWarn("There are no screens to go to.");
        return;
    }
    
    screenGoto(_screens[0]);
}

void VGDE::screenGotoLast() {
    if (_screens.empty()) {
        vWarn("There are no screens to go to.");
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
        vWarn("Already at the last screen.");
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
        vWarn("Already at the first screen.");
        return;
    }
    
    screenGoto(_screens[index - 1], cleanup);
}

size_t VGDE::screenCount() const {
    return _screens.size();
}

void VGDE::loggerSetName(const String &name) {
    _logger.reset();
    _logger = std::make_shared<spdlog::logger>(spdlog::logger(name.stdString(), {_consoleSink, _fileSink}));
    
    if (_loggerDebug) {
        _logger->set_level(spdlog::level::debug);
    }
}

void VGDE::loggerSetPattern(const String &pattern) {
    _loggerPattern = pattern;
    _logger->set_pattern(pattern.stdString());
}

std::shared_ptr<spdlog::logger> VGDE::logger() const {
    return _logger;
}

void VGDE::showVGDEDebugMessages(bool show) {
    if (show) {
        _vgdeLogger->set_level(spdlog::level::debug);
    } else {
        _vgdeLogger->set_level(spdlog::level::info);
    }
}

void VGDE::showDebugMessages(bool show) {
    _loggerDebug = show;
    
    if (_loggerDebug) {
        _logger->set_level(spdlog::level::debug);
    } else {
        _logger->set_level(spdlog::level::info);
    }
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
        vWarn("There are no screens to go to.");
        return false;
    }
    
    if (screenCount() == 1) {
        vWarn("There is only one screen.");
        return false;
    }
    
    if (_currentScreen == null) {
        vWarn("No screen currently selected. Going to the first screen.");
        screenGoto(_screens[0]);
        return false;
    }
    
    return true;
}
