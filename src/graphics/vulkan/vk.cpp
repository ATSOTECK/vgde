#include "vk.h"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <map>
#include <set>

namespace {
const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

VkResult loadDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *createInfo,
                                    const VkAllocationCallbacks *allocator, VkDebugUtilsMessengerEXT *debugMessenger)
{
    auto vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (vkCreateDebugUtilsMessengerEXT != null) {
        return vkCreateDebugUtilsMessengerEXT(instance, createInfo, allocator, debugMessenger);
    }

    vgderr("Could not load vkCreateDebugUtilsMessengerEXT!");
    return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                   const VkAllocationCallbacks *allocator)
{
    auto vkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (vkDestroyDebugUtilsMessengerEXT != null) {
        vkDestroyDebugUtilsMessengerEXT(instance, debugMessenger, allocator);
    }
}

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT msgSeverity,
                                             VkDebugUtilsMessageTypeFlagsEXT msgType,
                                             const VkDebugUtilsMessengerCallbackDataEXT *callbackData,
                                             void *userData)
{
    if (msgSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        vkWarn(callbackData->pMessage);
    } else if (msgSeverity > VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        vkErr(callbackData->pMessage);
    }

    return VK_FALSE;
}

void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    //Doesn't include VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData = null; // Optional
}

bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
    uint32 extensionCount;
    vkEnumerateDeviceExtensionProperties(device, null, &extensionCount, null);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, null, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto &extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}
}

Vulkan::~Vulkan() {
#ifdef VDEBUG
    destroyDebugUtilsMessengerEXT(_instance, _debugMessenger, null);
#endif

    for (auto imageView : _swapChainImageViews) {
        vkDestroyImageView(_device, imageView, null);
    }

    vkDestroySwapchainKHR(_device, _swapChain, null);
    vkDestroyDevice(_device, null);
    vkDestroySurfaceKHR(_instance, _surface, null);
    vkDestroyInstance(_instance, null);
}

void Vulkan::init(GLFWwindow *window, bool debug) {
    _window = window;

    createInstance();
    if (debug) {
        initDebug();
    }
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
    createSwapChain();
    createImageViews();
    createGraphicsPipeline();
}

void Vulkan::displayInfo() {
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(_physicalDevice, &properties);

    db(_deviceCount << " devices found.");
    db("Selected device: " << properties.deviceName);
}

void Vulkan::createInstance() {
#ifdef VDEBUG
    if (!checkValidationLayerSupport()) {
        vgderr("Validation layers requested but not available!");
    }
#endif

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(null, &extensionCount, null);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(null, &extensionCount, extensions.data());

    db(extensionCount << " available extensions:");
    for (const auto &extension : extensions) {
        db('\t' << extension.extensionName);
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "vgde";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    auto requiredExtensions = getRequiredExtensions();

    createInfo.enabledExtensionCount = requiredExtensions.size();
    createInfo.ppEnabledExtensionNames = requiredExtensions.data();

#ifdef VDEBUG
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
    createInfo.enabledLayerCount = staticCastu32(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();

    populateDebugMessengerCreateInfo(debugCreateInfo);
    createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
#else
    createInfo.enabledLayerCount = 0;
    createInfo.pNext = null;
#endif

    if (vkCreateInstance(&createInfo, null, &_instance) != VK_SUCCESS) {
        vgderr("Failed to create vulkan instance!");
    }
}

void Vulkan::initDebug() {
#ifdef VDEBUG
    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);

    if (loadDebugUtilsMessengerEXT(_instance, &createInfo, null, &_debugMessenger) != VK_SUCCESS) {
        vgderr("Failed to setup the debug messenger!");
    }
#endif
}

void Vulkan::createSurface() {
    if (glfwCreateWindowSurface(_instance, _window, null, &_surface) != VK_SUCCESS) {
        vgderr("Failed to create window surface!");
    }
}

void Vulkan::pickPhysicalDevice() {
    _physicalDevice = VK_NULL_HANDLE;
    _deviceCount = 0;
    vkEnumeratePhysicalDevices(_instance, &_deviceCount, null);

    if (_deviceCount == 0) {
        vgderr("Failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(_deviceCount);
    vkEnumeratePhysicalDevices(_instance, &_deviceCount, devices.data());

    std::multimap<int, VkPhysicalDevice> candidates;
    for (const auto &device : devices) {
        int score = rateDevice(device);
        candidates.insert({score, device});
    }

    if (candidates.rbegin()->first > 0) {
        _physicalDevice = candidates.rbegin()->second;
    }

    if (_physicalDevice == VK_NULL_HANDLE) {
        vgderr("Failed to find a suitable GPU!");
    }
}

void Vulkan::createLogicalDevice() {
    QueueFamilyIndices indices = findQueueFamilies(_physicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    float queuePriority = 1.f;
    for (uint32 queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = queueCreateInfos.size();
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = staticCastu32(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

#ifdef VDEBUG
    //For compatibility with older versions.
    createInfo.enabledLayerCount = staticCastu32(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
#else
    createInfo.enabledLayerCount = 0;
#endif

    if (vkCreateDevice(_physicalDevice, &createInfo, null, &_device) != VK_SUCCESS) {
        vgderr("Failed to create logical device!");
    }

    vkGetDeviceQueue(_device, indices.graphicsFamily.value(), 0, &_graphicsQueue);
    vkGetDeviceQueue(_device, indices.presentFamily.value(), 0, &_presentQueue);
}

void Vulkan::createSwapChain() {
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(_physicalDevice);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

    uint32 imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = _surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = findQueueFamilies(_physicalDevice);
    uint32 queueFamilyIndices[] = {staticCastu32(indices.graphicsFamily.value()), staticCastu32(indices.presentFamily.value())};

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = null;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(_device, &createInfo, null, &_swapChain) != VK_SUCCESS) {
        vgderr("Failed to create swap chain!");
    }

    vkGetSwapchainImagesKHR(_device, _swapChain, &imageCount, null);
    _swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(_device, _swapChain, &imageCount, _swapChainImages.data());

    _swapChainImageFormat = surfaceFormat.format;
    _swapChainExtent = extent;
}

void Vulkan::createImageViews() {
    _swapChainImageViews.resize(_swapChainImages.size());

    for (int i = 0; i < _swapChainImages.size(); ++i) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = _swapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = _swapChainImageFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(_device, &createInfo, null, &_swapChainImageViews[i]) != VK_SUCCESS) {
            vgderr("Failed to create image views!");
        }
    }
}

void Vulkan::createGraphicsPipeline() {
    //
}

bool Vulkan::checkValidationLayerSupport() {
    uint32 layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, null);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char *layerName : validationLayers) {
        bool found = false;

        for (const auto &layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                found = true;
                break;
            }
        }

        if (!found) {
            return false;
        }
    }

    return true;
}

std::vector<const char *> Vulkan::getRequiredExtensions() {
    uint32 glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

#ifdef VDEBUG
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

    return extensions;
}

QueueFamilyIndices Vulkan::findQueueFamilies(VkPhysicalDevice device) {
    QueueFamilyIndices indices;

    uint32 queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, null);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i;
    for (i = 0; i < queueFamilyCount; ++i) {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
            break;
        }
    }

    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, _surface, &presentSupport);

    if (presentSupport) {
        indices.presentFamily = i;
    }

    return indices;
}

int Vulkan::rateDevice(VkPhysicalDevice device) {
    int score = 0;
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(device, &properties);

    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures(device, &features);

    if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1000;
    }

    score += properties.limits.maxImageDimension2D;

    if (!features.geometryShader) {
        return 0;
    }

    QueueFamilyIndices indices = findQueueFamilies(device);
    if (!indices.isComplete()) {
        return 0;
    }

    if (!checkDeviceExtensionSupport(device)) {
        return 0;
    }

    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
    if (swapChainSupport.formats.empty() || swapChainSupport.presentModes.empty()) {
        return 0;
    }

    return score;
}

SwapChainSupportDetails Vulkan::querySwapChainSupport(VkPhysicalDevice device) {
    SwapChainSupportDetails details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, _surface, &details.capabilities);

    uint32 formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, _surface, &formatCount, null);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, _surface, &formatCount, details.formats.data());
    }

    uint32 presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, _surface, &presentModeCount, null);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, _surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

VkSurfaceFormatKHR Vulkan::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) {
    for (const auto &availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB
            && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    //TODO(Skyler): Should these be ranked?
    return availableFormats[0];
}

VkPresentModeKHR Vulkan::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> & availableModes) {
    for (const auto &mode : availableModes) {
        if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return mode;
        }
    }

    //Return the mode that is guaranteed to be available as a fallback.
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Vulkan::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities) {
    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    }

    int w, h;
    glfwGetWindowSize(_window, &w, &h);
    VkExtent2D actualExtent = {staticCastu32(w), staticCastu32(h)};

    actualExtent.width = std::max(capabilities.minImageExtent.width,
                                  std::min(capabilities.maxImageExtent.width, actualExtent.width));
    actualExtent.height = std::max(capabilities.minImageExtent.height,
                                   std::min(capabilities.maxImageExtent.height, actualExtent.height));

    return actualExtent;
}
