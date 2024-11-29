#include <window.hpp>

#include <globals.hpp>
#include <shader/vertex-shader.h>
#include <shader/fragment-shader.h>

// OpenGL Math Libraries
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.hpp>
#include <texture.hpp>
#include <camera.hpp>
#include <input.hpp>

#include <iostream>
#include <cmath>

namespace Marlin {
    GLFWwindow* window;
    std::pair<int, int> fb_dimensions = {800, 600};

    bool isMouseCaptured = false;

    // Callback function for resizing framebuffer upon window resize
    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
        fb_dimensions = {width, height};
    }

    // Frame time related things
    float currentFrame, lastFrame, deltaTime;

    // Render thread
    void renderThread() {
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