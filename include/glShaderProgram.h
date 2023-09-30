#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>


#include "GL/glew.h"
#include "log.h"

struct ShaderSources
{
    std::string VertexShader;
    std::string FragmentShader;
};

class GLShaderProgram
{
public:

    GLShaderProgram(const std::string& sourcesFilename)
    {
        ShaderSources shaderSources = ParseShader(sourcesFilename);

        m_rendererID = CreateShaderProgram(shaderSources.VertexShader, shaderSources.FragmentShader);
    }

    ~GLShaderProgram()
    {
        Unbind();
        GLCall(glDeleteProgram(m_rendererID));
    }

    void Bind() const
    {
        glUseProgram(m_rendererID);
    }

    void Unbind() const
    {
        glUseProgram(0);
    }

    void SetUniformMat4(const std::string& name, const glm::mat4& value)
    {
        GLuint location = GetUniformLocation(name);
        GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat*) &value));
    }

    void SetUniformVec2(const std::string& name, const glm::vec2& value)
    {
        GLuint location = GetUniformLocation(name);
        GLCall(glUniform2fv(location, 1, (const GLfloat*)&value));
    }

private:

    ShaderSources ParseShader(const std::string& filepath)
    {
        std::ifstream stream(filepath);

        enum class ShaderType
        {
            NONE = -1, VERTEX, FRAGMENT
        };

        std::string line;
        std::stringstream ss[2];
        ShaderType type = ShaderType::NONE;
        while (std::getline(stream, line))
        {
            if (line.find("#shader") != std::string::npos)
            {
                if (line.find("vertex") != std::string::npos)
                    type = ShaderType::VERTEX;
                else if (line.find("fragment") != std::string::npos)
                    type = ShaderType::FRAGMENT;
            }
            else
            {
                ss[(int)type] << line << '\n';
            }
        }

        return { ss[(int)ShaderType::VERTEX].str(), ss[(int)ShaderType::FRAGMENT].str() };
    }

    GLuint CompileShader(GLenum type, const std::string& source)
    {
        GLuint id = glCreateShader(type);
        const char* src = source.c_str();
        GLCall(glShaderSource(id, 1, &src, nullptr));
        GLCall(glCompileShader(id));

        int  success;
        char infoLog[512];
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(id, 512, NULL, infoLog);
            LOG_ERR(infoLog, success);
        }

        return id;
    }

    GLuint CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
    {

        GLCall(GLuint program = glCreateProgram());
        GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
        GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

        GLCall(glAttachShader(program, vs));
        GLCall(glAttachShader(program, fs));
        GLCall(glLinkProgram(program));
        GLCall(glValidateProgram(program));

        GLCall(glDeleteShader(vs));
        GLCall(glDeleteShader(fs));

        return program;
    }

    GLuint GetUniformLocation(const std::string& name) const
    {
        if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
            return m_uniformLocationCache[name];

        GLuint location = glGetUniformLocation(m_rendererID, name.c_str());
        m_uniformLocationCache[name] = location;
        return location;
    }

private:

    mutable std::unordered_map<std::string, GLuint> m_uniformLocationCache;
    GLuint m_rendererID;
};