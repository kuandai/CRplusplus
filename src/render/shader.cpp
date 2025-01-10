#include <render/shader.hpp>

#include <spdlog/spdlog.h>

#include <iostream>

namespace Marlin {
    // Reduce redundant code in shader compilation
    static unsigned int compileShader(unsigned int type, const char* source) {
        // Pad shader with null terminater
        const char *src = source + '\0';
        unsigned int shader;
        shader = glCreateShader(type);
        glShaderSource(shader, 1, &src, NULL);
        glCompileShader(shader); // Compile the shader

        // If compilation fails, indicate failure
        int  success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success) {
            int logLength;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
            char* infoLog = (char*)alloca(logLength * sizeof(char));
            glGetShaderInfoLog(shader, logLength, &logLength, infoLog);
            spdlog::error("Shader compilation failed.\n" + std::string(infoLog));
        }
        return shader;
    }

    Shader::Shader(const char* vertexSource, const char* fragmentSource) {
        // Vertex Shader
        unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);

        // Fragment Shader
        unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

        // Shader program
        Shader::ID = glCreateProgram();

        glAttachShader(Shader::ID, vertexShader);
        glAttachShader(Shader::ID, fragmentShader);
        glLinkProgram(Shader::ID);

        // Indicate failure if necessary
        int success;
        glGetProgramiv(Shader::ID, GL_LINK_STATUS, &success);
        if(!success) {
            char infoLog[512];
            glGetShaderInfoLog(Shader::ID, 512, NULL, infoLog);
            spdlog::error("Shader program linkage failed\n" + std::string(infoLog));
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
    void Shader::setMat4(const std::string &name, glm::mat4 &value) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
    }
}