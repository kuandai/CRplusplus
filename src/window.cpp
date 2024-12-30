#include <window.hpp>

#include <globals.hpp>
#include <shader/vertex-shader.h>
#include <shader/fragment-shader.h>

// OpenGL Math Libraries
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <spdlog/spdlog.h>

#include <shader.hpp>
#include <texture.hpp>
#include <camera.hpp>
#include <input.hpp>

#include <iostream>
#include <cmath>

namespace Marlin {
    std::pair<int, int> fb_dimensions = {800, 600};

    // Static members
    bool GameWindow::isMouseCaptured = false;

    // Input handling
    void GameWindow::processInput(GLFWwindow* window, float deltaTime) {
        // Simple camera motion
        float velocity = cameraSpeed * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            cameraPos += velocity * cameraFront;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            cameraPos -= velocity * cameraFront;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * velocity;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * velocity;
        }

        // Escape to toggle mouse capture
        int escKeyState = glfwGetKey(window, GLFW_KEY_ESCAPE);
        static int escKeyPrevState = GLFW_RELEASE;

        if (escKeyState == GLFW_PRESS & escKeyPrevState != GLFW_PRESS) {
            Marlin::GameWindow::isMouseCaptured = !Marlin::GameWindow::isMouseCaptured;
        }
        escKeyPrevState = escKeyState;
    }

    // Contructor and destructor
    GameWindow::GameWindow() {
        spdlog::info("Game window created");
        renderThread = std::thread(&GameWindow::renderLoop, this);
    }

    // GLFW window initialization
    int GameWindow::init() {
        // Initialize GLFW
        glfwInit();

        // OpenGL 3.3
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

        // Core profile: reduced feature set, no backwards compatibility
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create a window
        Marlin::GameWindow::window = glfwCreateWindow(800, 600, "Cosmic Reach++", NULL, NULL);
        if (Marlin::GameWindow::window == NULL)
        {
            spdlog::critical("Failed to create GLFW window");
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(Marlin::GameWindow::window);

        // Initialize GLAD
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            spdlog::critical("Failed to initialize GLAD");
            return -1;
        }

        // Set Viewport - should be same as window size
        glViewport(0, 0, 800, 600);

        // Register framebuffer resize callback
        glfwSetFramebufferSizeCallback(Marlin::GameWindow::window, [](GLFWwindow* window, int width, int height)
            {
                glViewport(0, 0, width, height);
                fb_dimensions = {width, height};
            });

        // Register keyboard update callback
        glfwSetKeyCallback(Marlin::GameWindow::window, [](GLFWwindow* window, int key, int scancode, int action, int mods) 
            {
                // Write all key events to `keyboard`
                if (key >= 0 && key < keyboard.size()) {
                    keyboard[key] = action;  // Store the action (GLFW_PRESS, GLFW_RELEASE)
                }
            });

        // Register mouse update callback
        glfwSetCursorPosCallback(Marlin::GameWindow::window, [](GLFWwindow* window, double xpos, double ypos) 
            {
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
            });

        // Set cursor mode
        glfwSetInputMode(Marlin::GameWindow::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        return 0;
    }

    // Render thread
    void GameWindow::renderLoop() {
        if (init()) { // If `init` fails (nonzero return)
            spdlog::critical("Render thread failed to initialize");
            return;
        }

        // Build shader program
        Shader ourShader(shader_vertex_shader_glsl, shader_fragment_shader_glsl);

        // Configure OpenGL Global State
        glEnable(GL_DEPTH_TEST);

        // Quick proof-of-concept
        float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
        };

        glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
        };
        
        unsigned int VBO, VAO;
        glGenBuffers(1, &VBO);
        glGenVertexArrays(1, &VAO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Position Attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Texture Coordinate Attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);


        // Example Texture
        Marlin::loadNewTexture("./textures/wall.jpg", "brick_wall");


        while(gameRunning) {
                // Calculate delta time since last frame
                currentFrame = glfwGetTime();
                deltaTime = currentFrame - lastFrame;
                lastFrame = currentFrame;

                // Process key inputs
                processInput(window, deltaTime);

                // Release/capture cursor as needed
                if (isMouseCaptured) { glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); }
                else { glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); }

                // Draw a background color
                glClearColor(0.2, 0.2, 0.2, 0.5f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Also clear depth buffer

                // Activate the shader
                ourShader.use();

                // Create transformations
                glm::mat4 model         = glm::mat4(1.0f);
                glm::mat4 view          = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
                glm::mat4 projection    = glm::mat4(1.0f);
                projection = glm::perspective(glm::radians(90.0f), (float)fb_dimensions.first / (float)fb_dimensions.second, 0.1f, 100.0f);

                // retrieve the matrix uniform locations
                unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
                unsigned int viewLoc  = glGetUniformLocation(ourShader.ID, "view");

                // pass them to the shaders (3 different ways)
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
                ourShader.setMat4("projection", projection);

                // Bind the texture - This assigns it to the Sampler2D in the fragment shader
                glBindTexture(GL_TEXTURE_2D, Marlin::textureAtlas["brick_wall"].texture);

                // Bind the VAO
                glBindVertexArray(VAO);


                // Draw the cube
                glDrawArrays(GL_TRIANGLES, 0, 36);

                // Gets all kbd + mouse events, calls appropriate callback functions
                glfwPollEvents();


                // Swaps the framebuffers (the frame should be done rendering now)
                glfwSwapBuffers(window);

                // Stop game if window is closed
                if (glfwWindowShouldClose(window)) {
                    gameRunning = false;
                }
        }

        // Clean up resources & exit
        glfwTerminate(); // Deallocate GLFW resources
    }
}