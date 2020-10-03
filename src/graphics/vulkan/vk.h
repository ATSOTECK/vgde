#ifndef __VGDE_VK_H__
#define __VGDE_VK_H__

#include "config.h"
#include "vec.h"

#include <array>
#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>
#include <optional>
#include <string>
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

struct Vertex {
    vec2f pos;
    vec3f color;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription;
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);
        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        return attributeDescriptions;
    }
};

void vkResize(GLFWwindow *, int, int);

class Vulkan {
public:
    ~Vulkan();
    void init(GLFWwindow *window, bool debug = false);

    void drawFrame();
    void wait();

    void displayInfo();

private:
    void createInstance();
    void initDebug();
    void createSurface();
    void pickPhysicalDevice();
    void createLogicalDevice();

    void cleanupSwapChain();
    void recreateSwapChain();

    void createSwapChain();
    void createImageViews();
    void createRenderPass();
    void createGraphicsPipeline();
    void createFramebuffers();
    void createCommandPool();
    void createCommandBuffers();

    void createSyncObjects();

    void createVertexBuffer();

    static bool checkValidationLayerSupport();
    static std::vector<const char *> getRequiredExtensions();

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    int rateDevice(VkPhysicalDevice device);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
    static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> & availableModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

    static std::vector<char> readFile(const std::string &filename);

    VkShaderModule createShaderModule(const std::vector<char> &code);

    uint32 findMemoryType(uint32 filter, VkMemoryPropertyFlags properties);

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
    VkRenderPass _renderPass;
    VkPipelineLayout _pipelineLayout;
    VkPipeline _graphicsPipeline;
    std::vector<VkFramebuffer> _swapChainFramebuffers;
    VkCommandPool _commandPool;
    std::vector<VkCommandBuffer> _commandBuffers;
    std::vector<VkSemaphore> _imageAvailableSemaphores;
    std::vector<VkSemaphore> _renderFinishedSemaphores;
    std::vector<VkFence> _inFlightFences;
    std::vector<VkFence> _imagesInFlight;
    size_t _currentFrame;
    VkBuffer _vertexBuffer;
    VkDeviceMemory _vertexBufferMemory;

    std::vector<Vertex> _verts;
};
#endif //__VGDE_VK_H__
