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

#include "tst/game/tstSplashScreen.h"
#include "tst/game/tstGameScreen.h"

#include "audio/al/alSoundDevice.h"

/*
#include <GLFW/glfw3.h>
#include "graphics/vulkan/vk.h"

int main() {
    glfwInit();
    
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow *window = glfwCreateWindow(800, 600, "vk", null, null);
    glfwSetFramebufferSizeCallback(window, vkResize);
    
    Vulkan vk{};
    vk.init(window, VK_DEBUG);
    vk.displayInfo();
    
    int frames = 0;
    int time = Clock::timeAsMilliseconds();
    
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        vk.drawFrame();
        ++frames;
        
        if (Clock::timeAsMilliseconds() > time + 1000) {
            time = Clock::timeAsMilliseconds();
            db("fps: " << frames);
            frames = 0;
        }
    }
    
    vk.wait();
    
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}
*/

#include <fstream>
#define TOML_EXCEPTIONS 0
#include <toml.hpp>

int main() {
    SetConsoleOutputCP(CP_UTF8);
    AlSoundDevice *al = AlSoundDevice::instance();
    
    toml::parse_result result = toml::parse_file("../tst.toml");
    if (!result) {
        db("error");
        return 0;
    }
    
    var table = result.table();
    db(table);
    var opt = table["vgde"]["fullscreen"].value<bool>();
    if (opt.has_value()) {
        db(opt.value());
    }
    
    //return 0;
    ResourceManager::instance()->setResPath("../res");
    VGDE *vgde = VGDE::instance();
    //vgde->init(vgde->nativeVideoMode());
    vgde->init();
    vgde->loggerSetName("test game");
    vgde->loggerSetPattern("[%I:%M:%S %p] [%n] [%^-%L-%$] %v");
    vgde->screenAdd(new TstSplashScreen());
    vgde->screenAdd(new TstGameScreen());
    vgde->screenGotoFirst();
    //vgde->screenGoto("splash");
    
    vgde->run();
    
    return 0;
}
