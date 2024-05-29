#ifndef SHADER_H
#define SHADER_H

#include <glew.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
    GLuint programID;

    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr) {
        std::string vertexCode = readShaderFile(vertexPath);
        std::string fragmentCode = readShaderFile(fragmentPath);
        std::string geometryCode = geometryPath ? readShaderFile(geometryPath) : "";

        GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexCode.c_str());
        GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentCode.c_str());
        GLuint geometryShader = geometryPath ? compileShader(GL_GEOMETRY_SHADER, geometryCode.c_str()) : 0;

        programID = glCreateProgram();
        glAttachShader(programID, vertexShader);
        glAttachShader(programID, fragmentShader);
        if (geometryPath) {
            glAttachShader(programID, geometryShader);
        }
        linkProgram(programID);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        if (geometryPath) {
            glDeleteShader(geometryShader);
        }
    }

    void use() const {
        glUseProgram(programID);
    }

    GLuint getID() const {
        return programID;
    }

    void setBool(const std::string& name, bool value) const {
        glUniform1i(glGetUniformLocation(programID, name.c_str()), static_cast<int>(value));
    }

    void setInt(const std::string& name, int value) const {
        glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
    }

    void setFloat(const std::string& name, float value) const {
        glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
    }

    void setVec2(const std::string& name, const glm::vec2& value) const {
        glUniform2fv(glGetUniformLocation(programID, name.c_str()), 1, glm::value_ptr(value));
    }

    void setVec3(const std::string& name, const glm::vec3& value) const {
        glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1, glm::value_ptr(value));
    }

    void setVec4(const std::string& name, const glm::vec4& value) const {
        glUniform4fv(glGetUniformLocation(programID, name.c_str()), 1, glm::value_ptr(value));
    }

    void setMat2(const std::string& name, const glm::mat2& mat) const {
        glUniformMatrix2fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }

    void setMat3(const std::string& name, const glm::mat3& mat) const {
        glUniformMatrix3fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }

    void setMat4(const std::string& name, const glm::mat4& mat) const {
        glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }

private:
    std::string readShaderFile(const char* path) {
        std::ifstream shaderFile;
        shaderFile.open(path);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        return shaderStream.str();
    }

    GLuint compileShader(GLenum shaderType, const char* shaderSource) {
        GLuint shader = glCreateShader(shaderType);
        glShaderSource(shader, 1, &shaderSource, nullptr);
        glCompileShader(shader);

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        return shader;
    }

    void linkProgram(GLuint program) {
        glLinkProgram(program);
        GLint success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(program, 512, nullptr, infoLog);
            std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
    }
};

#endif // SHADER_H
