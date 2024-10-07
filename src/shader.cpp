#include <shader.hpp>

#include <iostream>

namespace Marlin {
    // Compile vertex and fragment shaders, then link
    Shader::Shader(const char* vertexSource, const char* fragmentSource) {
        // Vertex Shader
        const char *vertexShaderSource = vertexSource + '\0';
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
        const char *fragmentShaderSource = fragmentSource + '\0';
        unsigned int fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        // If compilation fails, indicate failure
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

        if(!success) {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        // Shader program
        Shader::ID = glCreateProgram();

        glAttachShader(Shader::ID, vertexShader);
        glAttachShader(Shader::ID, fragmentShader);
        glLinkProgram(Shader::ID);

        // Once again, indicate failure if necessary
        glGetProgramiv(Shader::ID, GL_LINK_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(Shader::ID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKAGE_FAILED\n" << infoLog << std::endl;
        }

        // Delete vertex and fragment shader objects; already linked
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    // Method to activate shader
    void Shader::use() {
        glUseProgram(ID);
    }

    void Shader::setBool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }

    void Shader::setInt(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void Shader::setFloat(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
}