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

#ifndef __VGDE_VGDE_H__
#define __VGDE_VGDE_H__

#include "graphics/gl.h"
#include <string>
#include <vector>

#include "config.h"
#include "vec.h"
#include "game/screen.h"
#include "graphics/videoMode.h"
#include "util/clock.h"
#include "util/vstring.h"

static const int DEFAULT_WINDOW_WIDTH  = 1024;
static const int DEFAULT_WINDOW_HEIGHT = 600;

class VGDE {
public:
	static VGDE *instance();

	int init();
	int init(VideoMode mode);
	int init(int width, int height, const std::string &title, bool fullScreen = false);
	
	void run();

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
	
	vec2f windowCenter() const;

	std::string windowTitle() const;
	void setWindowTitle(const std::string &title);

	void windowMaximize() const;

	int fps();
	int32 frameTime() const;

	float inGameTime() const;
	float totalInGameTime() const;
	
	Screen *currentScreen() const;
	void addScreen(Screen *screen);
	void gotoScreen(Screen *screen, bool cleanup = false);
	void gotoScreen(const String &screen, bool cleanup = false);
	size_t screenCount() const;

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
	
	std::vector<Screen *> _screens;
	Screen *_currentScreen;
};

#endif
