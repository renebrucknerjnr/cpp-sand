#include <GLFW/glfw3.h>
#include <iostream>

bool leftMouseHeld = false;
double mx, my;
const int WIDTH = 640;
const int HEIGHT = 480;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            leftMouseHeld = true;
        } else if (action == GLFW_RELEASE) {
            leftMouseHeld = false;
        }
    }
}


int main()
{
    if (!glfwInit()) {
        std::cerr << "GLFW init failed\n";
        return -1;
    }

    // IMPORTANT: disable OpenGL
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // glfw cannot draw pixels without an api (opengl, windows.h, vulkan, etc.)
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window =
        glfwCreateWindow(WIDTH, HEIGHT, "Sand Simulation", nullptr, nullptr);

    if (!window) {
        std::cerr << "Window creation failed\n";
        glfwTerminate();
        return -1;
    }

    // link function to glfw for mouse button (event listener)
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glfwGetCursorPos(window, &mx, &my);

        if (leftMouseHeld) {
            std::cout << "Mouse held at: " << mx << ", " << my << "\n";
            // putPixel((int)mx, (int)my, 0xFFFF0000); // draw pixel
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}




// pacman -Syu
// pacman -S mingw-w64-x86_64-glfw
// pacman -S mingw-w64-x86_64-vulkan-devel

// cd "/c/users/administrator/desktop/glfw sandsim"; clear; g++ -o run.exe "main (no opengl).cpp" -I"C:/msys64/mingw64/include" -L"C:/msys64/mingw64/lib" -lglfw3 -lgdi32 -luser32 -static-libgcc -static-libstdc++ -static; ./run.exe
