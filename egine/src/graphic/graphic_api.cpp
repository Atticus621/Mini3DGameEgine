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

std::shared_ptr<engine::ShaderProgram>& engine::GraphicAPI::GetDefaultShaderProgram()
{
    if (!m_defaultShaderProgram) {
        std::string vertexShader = R"(#version 300 es
    precision mediump float;
    layout (location = 0) in vec3 position;
    layout (location = 1) in vec3 color;
    layout (location = 2) in vec2 texCoord;
    layout (location = 3) in vec3 normal;

    out vec2 vTexCoord;
    out vec3 vNormal;
    out vec3 vFragPos;

    uniform mat4 uModel;
    uniform mat4 uView;
    uniform mat4 uProjection;

    void main() {
        vFragPos = vec3(uModel * vec4(position,1.0));
        vNormal = mat3(transpose(inverse(uModel))) *normal;
        vTexCoord=texCoord;
        gl_Position = uProjection * uView * uModel * vec4(position,1.0);
    }
)";
        std::string fragmentShaderSource = R"(#version 300 es
precision mediump float;

struct Light{
    vec3 position;
    vec3 color;
};
uniform Light uLight;
uniform sampler2D baseColorTexture;
uniform vec3 uCameraPosition;

in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vFragPos;
out vec4 FragColor;

void main() {
    // 归一化法向量
    vec3 norm = normalize(vNormal);
    // 计算光源方向并归一化
    vec3 lightDir = normalize(uLight.position - vFragPos);
    
    // 漫反射计算
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * uLight.color;
    
    // 镜面反射计算
    vec3 viewDir = normalize(uCameraPosition - vFragPos);
    // 修复1：LightDir → lightDir（大小写错误）
    vec3 reflectDir = reflect(-lightDir, norm);
    // 修复2：32 → 32.0（pow需要浮点参数）
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    float specularStrength = 0.5;
    vec3 specular = specularStrength * spec * uLight.color;
    
    // 修复3：声明result变量（vec3类型，匹配diffuse/specular）
    vec3 result = diffuse + specular;
    
    // 采样纹理颜色
    vec4 texColor = texture(baseColorTexture, vTexCoord);
    // 最终颜色 = 纹理颜色 * 光照颜色（加上alpha通道）
    FragColor = texColor * vec4(result, 1.0);
})";
        m_defaultShaderProgram = CreateShaderProgram(vertexShader, fragmentShaderSource);
    }
    return m_defaultShaderProgram;
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
    else {
        material->Bind();
    }

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
