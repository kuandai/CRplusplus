#include <input.hpp>

#include <iostream>

#include <render/window.hpp>
#include <render/camera.hpp>

namespace Marlin{
    std::vector<int> keyboard(GLFW_KEY_LAST+1, GLFW_RELEASE);

    void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        // Write all key events to `keyboard`
        if (key >= 0 && key < keyboard.size()) {
            keyboard[key] = action;  // Store the action (GLFW_PRESS, GLFW_RELEASE)
        }
    }

    void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
        // Inital Position for mouse: center of screen
        static float lastX = 400, lastY = 300;
        static bool firstmouse = true;

        if (firstmouse) {
            lastX = xpos;
            lastY = ypos;
            firstmouse = false;
        }

        // If cursor enabled, do not update camera
        if (!Marlin::GameWindow::isMouseCaptured) { return; }

        float xOffset = xpos - lastX;
        float yOffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to top
        lastX = xpos;
        lastY = ypos;

        xOffset *= sensitivity;
        yOffset *= sensitivity;

        yaw += xOffset;
        pitch += yOffset;

        // Constrain the pitch to prevent screen flipping
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(front);
    }
}