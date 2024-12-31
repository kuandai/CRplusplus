#pragma once

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include <GLFW/glfw3.h>

#include <thread>
#include <atomic>

namespace Marlin {
    class GameWindow {
        public:
            // Constructor and Destructor
            GameWindow();
            ~GameWindow();
            
            // Window/interface state
            static bool isMouseCaptured;

        private:
            // Frame time
            float currentFrame, lastFrame, deltaTime;

            // The Window
            GLFWwindow* window;
            
            // Input processing
            static void processInput(GLFWwindow* window, float deltaTime);

            // Render Thread
            int init();
            void renderLoop();
            std::thread renderThread;
    };
}