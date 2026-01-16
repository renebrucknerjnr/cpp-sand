#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

int main()
{
    // 1. Init GLFW
    if (!glfwInit()) {
        std::cerr << "GLFW init failed\n";
        return -1;
    }

    // 2. Tell GLFW we are using Vulkan (NO OpenGL)
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    GLFWwindow* window =
        glfwCreateWindow(640, 480, "GLFW + Vulkan", nullptr, nullptr);

    if (!window) {
        std::cerr << "Window creation failed\n";
        glfwTerminate();
        return -1;
    }

    // 3. Get required Vulkan extensions from GLFW
    uint32_t extCount = 0;
    const char** extensions = glfwGetRequiredInstanceExtensions(&extCount);

    if (!extensions) {
        std::cerr << "Vulkan not supported\n";
        return -1;
    }

    // 4. Create Vulkan instance
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "GLFW Vulkan App";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = extCount;
    createInfo.ppEnabledExtensionNames = extensions;

    VkInstance instance;
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        std::cerr << "Failed to create Vulkan instance\n";
        return -1;
    }

    std::cout << "Vulkan initialized successfully\n";

    // 5. Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    // 6. Cleanup
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}



// pacman -Syu
// pacman -S mingw-w64-x86_64-glfw
// pacman -S mingw-w64-x86_64-vulkan-devel

// cd "/c/users/administrator/desktop/glfw sandsim"; clear; g++ -o run.exe "main (vulkan).cpp" -I"C:/msys64/mingw64/include" -L"C:/msys64/mingw64/lib" -lglfw3 -lvulkan-1 -lgdi32 -luser32 -static-libgcc -static-libstdc++; ./run.exe
