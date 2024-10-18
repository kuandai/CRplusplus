#include <window.hpp>

#include <globals.hpp>
#include <shader/vertex-shader.h>
#include <shader/fragment-shader.h>

#include <shader.hpp>
#include <texture.hpp>

#include <iostream>
#include <cmath>

namespace Marlin {
    GLFWwindow* window;

    // Callback function for resizing framebuffer upon window resize
    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    // Render thread
    void renderThread() {
        Shader ourShader(shader_vertex_shader_glsl, shader_fragment_shader_glsl);

        // Quick proof-of-concept
        float vertices[] = { // EBO reduces footprint of verticies
            // Positions        // Colors         // Texture coordinates
            0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 2.0f, 2.0f,   // top right
            0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 2.0f, 0.0f,   // bottom right
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,   // bottom left
            -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 2.0f    // top left
        };
        unsigned int indices[] = {  // note that we start from 0!
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
        }; 
        
        unsigned int VBO, VAO, EBO;
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glGenVertexArrays(1, &VAO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Position Attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Color Attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
        glEnableVertexAttribArray(1);

        // Texture Coordinate Attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);


        // Example Texture
        Marlin::loadNewTexture("./textures/wall.jpg", "brick_wall");
        

        while(gameRunning) {
                // Draw a background color
                glClearColor(0.2f, 0.3f, 0.3f, 0.5f);
                glClear(GL_COLOR_BUFFER_BIT);

                // Activate the shader
                ourShader.use();

                // Bind the texture - This assigns it to the Sampler2D in the fragment shader
                glBindTexture(GL_TEXTURE_2D, Marlin::textureAtlas["brick_wall"].texture);

                // Bind the VAO
                glBindVertexArray(VAO);

                // Update uniform color
                float timeValue = glfwGetTime();
                float greenValue = sin(timeValue) / 2.0f + 0.5f;
                int vertexColorLocation = glGetUniformLocation(ourShader.ID, "ourColor");
                glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

                // Draw the shape
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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