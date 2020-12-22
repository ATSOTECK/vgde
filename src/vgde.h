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
#include "util/resourceManager.h"
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

	keep bool running() const;
	void preRender();
	void postRender();
	void cleanUp();

	void exit();

	keep std::vector<VideoMode> videoModes() const;
	keep VideoMode videoMode() const;
	keep VideoMode nativeVideoMode() const;
	
	keep bool windowVSync() const;
	void windowSetVSync(bool vsync);

	keep vec2f windowSize() const;
	void windowSetSize(const vec2f &size);
	
	keep float windowWidth() const;
	keep float windowHeight() const;
	
	keep vec2f windowCenter() const;

	keep std::string windowTitle() const;
	void windowSetTitle(const std::string &title);

	void windowMaximize() const;
	
	keep bool windowFocused() const;
	
	keep bool pauseIfNotFocused() const;
	void pauseIfNotFocused(bool pause);

	int fps();
	keep int32 frameTime() const;
	keep float delta() const;

	keep float inGameTime() const;
	keep float totalInGameTime() const;
	
	void screenshotSetName(const String &name);
	void screenshot();
	
	keep Screen *screen() const;
	void screenAdd(Screen *screen);
	void screenGoto(Screen *screen, bool cleanup = false);
	void screenGoto(const String &screen, bool cleanup = false);
	void screenGoto(int index, bool cleanup = false);
	void screenGotoFirst();
	void screenGotoLast();
	void screenGotoNext(bool cleanup = false);
	void screenGotoPrevious(bool cleanup = false);
	keep size_t screenCount() const;

private:
	friend void windowSizeCallback(GLFWwindow *, int, int);
	friend void windowCloseCallback(GLFWwindow *);
	friend void windowFocusCallback(GLFWwindow *, int);

	VGDE();
	static VGDE *_instance;

    void glInit() const;
	void resize(float w, float h);

	void saveInGameTime() const;
	void loadInGameTime();
    
    bool screenCheckPN();

	bool _initialized;
	GLFWwindow *_window;
	bool _vsync;
	float _windowWidth;
	float _windowHeight;
	std::string _windowTitle;
	bool _fullScreen;
	bool _pauseINF;

	int _frames;
	int _frameRate;
	int _time;
	Clock _clock;
	uint32 _frameTime;
	float _delta;
	float _startTime;
	float _totalInGameTime;
	
	std::vector<Screen *> _screens;
	Screen *_currentScreen;
	String _ssname;
	
	int _textureSlots;
	
	ResourceManager *_rm;
};

#endif
