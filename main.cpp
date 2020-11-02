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
    ResourceManager::instance()->setResPath("../res");
	VGDE *vgde = VGDE::instance();
	//vgde->init(vgde->nativeVideoMode());
	vgde->init();
    vgde->screenAdd(new TstSplashScreen());
    vgde->screenAdd(new TstGameScreen());
    vgde->screenGotoFirst();
    //vgde->screenGoto("splash");
	vgde->run();
	
	return 0;
}//*/
