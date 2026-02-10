#include "graphic_api.h"
#include "shaderProgram.h"
#include "render/material.h"
#include "render/mesh.h"

#include <spdlog/spdlog.h>

std::shared_ptr<engine::ShaderProgram> engine::GraphicAPI::CreateShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
{
    // Compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShaderStr = vertexShaderSource.c_str();
    glShaderSource(vertexShader, 1, &vertexShaderStr, NULL);
    glCompileShader(vertexShader);

    // Check vertex shader compilation
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        spdlog::error("ERROR::SHADER::VERTEX::COMPILATION_FAILED{}", infoLog);
        return nullptr;
    }

    // Compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentShaderStr = fragmentShaderSource.c_str();
    glShaderSource(fragmentShader, 1, &fragmentShaderStr, NULL);
    glCompileShader(fragmentShader);

    // Check fragment shader compilation
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        spdlog::error("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED{}", infoLog);
        return nullptr;
    }

    // Create shader program
    GLuint shaderProgramID = glCreateProgram();
    glAttachShader(shaderProgramID, vertexShader);
    glAttachShader(shaderProgramID, fragmentShader);
    glLinkProgram(shaderProgramID);

    // Check shader program linking
    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
        spdlog::error("ERROR::SHADER::PROGRAM::LINKING_FAILED{}" ,infoLog);

        return nullptr;
    }

    // Cleanup shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return std::make_shared<ShaderProgram>(shaderProgramID);
}

bool engine::GraphicAPI::Init()
{
	glEnable(GL_DEPTH_TEST);
    return true;
}

void engine::GraphicAPI::BindShaderProgram(engine::ShaderProgram* shaderProgram)
{
    if (!shaderProgram) {
        spdlog::error("shaderProgram is nullptr,can not bind SahderProgram");
    }
    shaderProgram->Bind();
}

void engine::GraphicAPI::BindMaterial(engine::Material* material)
{
    if (!material) {
        spdlog::error("material is nullptr,can not bind material");
    }
    material->Bind();
}

GLuint engine::GraphicAPI::CreateVBO(const std::vector<float>& vertices)
{
    GLuint VBO =0;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return VBO;
}

GLuint engine::GraphicAPI::CreateEBO(const std::vector<uint32_t>& indices)
{
    GLuint EBO=0;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    return EBO;
}

void engine::GraphicAPI::SetClearCorlor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void engine::GraphicAPI::ClearBuffer()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void engine::GraphicAPI::BindMesh(Mesh* mesh)
{
    if (!mesh) {
        spdlog::error("mesh is nullptr, graphic api can not bind");

    }
    mesh->Bind();
}

void engine::GraphicAPI::DrawMesh(Mesh* mesh)
{
    if (!mesh) {
        spdlog::error("mesh is nullptr, graphic api can not draw");

    }
    mesh->Draw();
}
