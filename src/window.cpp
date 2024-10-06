#include <window.hpp>

#include <globals.hpp>
#include <shader/vertex-shader.h>
#include <shader/fragment-shader.h>

#include <iostream>

namespace Marlin {
    GLFWwindow* window;

    // Callback function for resizing framebuffer upon window resize
    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    // Render thread
    void renderThread() {
        // Vertex Shader
        const char *vertexShaderSource = shader_vertex_shader_glsl + '\0';
        unsigned int vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader); // Compile the shader

        // If compilation fails, indicate failure
        int  success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

        if(!success) {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        // Fragment Shader
        const char *fragmentShaderSource = shader_fragment_shader_glsl + '\0';
        unsigned int fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        // If compilation fails, indicate failure
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

        if(!success) {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        // Shader program
        unsigned int shaderProgram;
        shaderProgram = glCreateProgram();

        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        // Once again, indicate failure if necessary
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        // Delete vertex and fragment shader objects; already linked
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // Quick proof-of-concept
        float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f
        };
        
        unsigned int VBO, VAO;
        glGenBuffers(1, &VBO); 
        glGenVertexArrays(1, &VAO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);  


        while(gameRunning) {
                // Draw a background color
                glClearColor(0.2f, 0.3f, 0.3f, 0.5f);
                glClear(GL_COLOR_BUFFER_BIT);

                // Activate the shader
                glUseProgram(shaderProgram);

                // Bind the VAO
                glBindVertexArray(VAO);

                // First Triangle!
                glDrawArrays(GL_TRIANGLES, 0, 3);

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