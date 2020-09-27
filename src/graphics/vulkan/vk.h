#ifndef __VGDE_VK_H__
#define __VGDE_VK_H__

#include "config.h"

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>
#include <optional>
#include <vector>
#include <vulkan.h>
#include <Windows.h>

//Un-define the windows versions of std::max and std::min
#undef max
#undef min

#define vkErr(x) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); std::cerr << "Validation Layer Error: " << x << std::endl; SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15)
#define vkWarn(x) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); std::cerr << "Validation Layer Warning: " << x << std::endl; SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15)

#define VK_DEBUG true

struct QueueFamilyIndices {
    [[nodiscard]] bool isComplete() const {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }

    std::optional<uint32> graphicsFamily;
    std::optional<uint32> presentFamily;
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class Vulkan {
public:
    ~Vulkan();
    void init(GLFWwindow *window, bool debug = false);

    void displayInfo();

private:
    void createInstance();
    void initDebug();
    void createSurface();
    void pickPhysicalDevice();
    void createLogicalDevice();
    void createSwapChain();
    void createImageViews();
    void createGraphicsPipeline();

    static bool checkValidationLayerSupport();
    static std::vector<const char *> getRequiredExtensions();

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    int rateDevice(VkPhysicalDevice device);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
    static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> & availableModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

    VkInstance _instance;
    VkDebugUtilsMessengerEXT _debugMessenger;
    GLFWwindow *_window;
    VkSurfaceKHR _surface;
    VkPhysicalDevice _physicalDevice;
    uint32 _deviceCount;
    VkDevice _device;
    VkQueue _graphicsQueue;
    VkQueue _presentQueue;
    VkSwapchainKHR _swapChain;
    std::vector<VkImage> _swapChainImages;
    VkFormat _swapChainImageFormat;
    VkExtent2D _swapChainExtent;
    std::vector<VkImageView> _swapChainImageViews;
};
#endif //__VGDE_VK_H__
