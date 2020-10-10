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

#include "config.h"

#include <iostream>
//#include "util/clock.h"

///*
#include "vgde.h"
#include "graphics/draw.h"

#include "input.h"

#include "util/vmath.h"
#include "util/clock.h"
#include "util/humanReadable.h"
#include "util/resourceManager.h"

#include "graphics/renderTarget.h"

#include "tst/game/tstSplashScreen.h"
#include "tst/game/tstGameScreen.h"
 //*/
/*
#include <GLFW/glfw3.h>
#include "graphics/vulkan/vk.h"

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(800, 600, "vgde vulkan", nullptr, nullptr);
    glfwSetFramebufferSizeCallback(window, vkResize);

    Vulkan vulkan{};
    vulkan.init(window, VK_DEBUG);
    vulkan.displayInfo();

    int frames = 0;
    Clock clock;
    int32 time = clock.restart().asMilliseconds();

    while(!glfwWindowShouldClose(window)) {
        clock.restart();
        glfwPollEvents();

        vulkan.drawFrame();
        ++frames;

        if (Clock::timeAsMilliseconds() > time + 1000) {
            time = Clock::timeAsMilliseconds();
            //db("fps: " << frames);
            frames = 0;
        }
    }

    vulkan.wait();

    glfwDestroyWindow(window);

    glfwTerminate();

    //std::cin.get();
    return 0;
}*/

///*
int main() {
    String w = "hello world";
    String h = "hello world";
    db(w.startsWith("hello"));
    db(w.endsWith("world"));
    
    db((w == h));
    
    w.erase(4, 2);
    db(w);
    w.erase(0);
    db(w);
    String s = "    yo    ";
    db("sw " << s.startsWith("yo", true));
    db("sw " << s.startsWith("yo"));
    
    db("ew " << s.endsWith("yo", true));
    db("ew " << s.endsWith("yo"));
    
    db("'" << s << "'");
    s.trimLeadingWhitespace();
    db("'" << s << "'");
    s.trimTrailingWhitespace();
    db("'" << s << "'");
    
    String c("我爱你 :)");
    db(c);
    db(c.length());
    db(c.size());
    for (int i = 0; i < c.length(); ++i) {
        uint32 cp = c[i];
        db(cp);
    }
    //return 0;
    
    ResourceManager::instance()->setResPath("../res");
	VGDE *vgde = VGDE::instance();
	//vgde->init(vgde->nativeVideoMode());
	vgde->init();
	vgde->addScreen(new TstSplashScreen());
    vgde->addScreen(new TstGameScreen());
    vgde->gotoScreen("splash");
	vgde->run();
	
	return 0;

	drawSetColor(Color::Red);
	//drawSetClearColor(Color::Blue);
	
    Sprite fire("fire.png");
	fire.setPosition({100, 200});
	fire.setSize({32, 32});
	fire.setRotation(90);

	Sprite f("tst.png");
	f.setPosition({700, 400});
    //f.setScale({0.5f});
	f.setOrigin(f.center());
	//f.setScale({0.5f});

	std::vector<Sprite *> sprs;
	sprs.reserve(1000);
	for (int i = 0; i < 1000; ++i) {
	    var t = new Sprite("fire.png");
	    t->setSize({32, 32});
	    t->setPosition(randomPosition(700, 500));
	    t->setOrigin(t->center());
	    t->setRotation(random(360));
	    sprs.push_back(t);
	}
	
	Font *chinese = new Font("SimSun.ttf");

	RenderTarget rt({100, 100});

	vgdewarn("warn yo");
	here
	dbln
	trace("ye");
	tracemsg << "haw\n";

	while (vgde->running()) {
	    vgde->preRender();
		
		f.rotate(1.f);
		f.move({-.5f, 0.f});
		
        fire.draw();
        f.draw();

        for (var s : sprs) {
            //s->rotate(random(2));
            //s->setScale(random(2) / 8);
            //s->setOrigin(s->center());
            //s->draw();
        }

		drawSetColor(Color::Red);
		drawLine(10, 10, 100, 100);

		drawSetColor(Color::Blue);
		drawLine(300, 0, 300, 23);

        drawSetColor(Color::White);
        drawLine(0, 0, f.position().x, f.position().y);

		drawRectangle(400, 500, 100, 35);
		drawSetColor(Color::Red + Color::Green);
		drawRectangle(400, 500, 100, 35, true);

		//vgde->setWindowTitle("vgde " + std::to_string(vgde->fps()));

		std::string txt = "fps: " + std::to_string(vgde->fps()) + "\t[blue]" + std::to_string(vgde->frameTime()) + "ms\n"
		        + "[cyan]" + secondsToHHMMSS((int)vgde->totalInGameTime());
		drawText(txt, 0, 0, 1, Color::Green);

        drawText("The quick brown fox jumped over the lazy dog", 200, 50, 1, Color::Red);

        drawText("我当然还是[red]爱[]你", 800, 100, 1, Color::White, chinese);
        //drawText(String("[blue]:)"), 800, 100, 1, Color::White);

        //glm::mat4 proj = drawGetProjection();
        //proj = glm::translate(proj, {.5f, 0.f, 0.f});
        //drawSetProjection(proj);

		if (isKeyDown(vk_space)) {
			setMousePosition(vec2f(100, 100));
		} else if (isKeyDown(vk_z)) {
			setMousePositionGlobal(vec2f(100, 100));
		}

		vgde->postRender();
	}

	vgde->cleanUp();
	return 0;
}//*/
