#include "game.h"

#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>
#include <string>

bool Game::Init()
{
    // Shader sources
    std::string vertexShaderSource = R"(#version 300 es
    layout (location = 0) in vec2 aPos;
    uniform vec2 uOffset;
    uniform vec2 uScale;
    
    void main() {
        vec2 position = (aPos * uScale) + uOffset;
        gl_Position = vec4(position, 0.0, 1.0);
    }
)";

    std::string fragmentShaderSource = R"(#version 300 es
    precision mediump float;
    out vec4 FragColor;
    uniform vec3 uColor;
    
    void main() {
        FragColor = vec4(uColor, 1.0);
    }
)";

    auto& graphicAPI = engine::Engine::GetInstance().GetGraphicAPI();
    auto shaderProgram = graphicAPI.CreateShaderProgram(vertexShaderSource, fragmentShaderSource);

    spdlog::info("create shaderProgram :{}",static_cast<unsigned int>(shaderProgram->GetShaderProgramID()));

    m_material.SetShaderProgram(shaderProgram);
	return true;
}

void Game::Update(float delta)
{
	auto& input = engine::Engine::GetInstance().GetInputManager();
	if (input.GetKeyPressed(GLFW_KEY_Q)) {
		spdlog::info("pressed Q");
	}
	

}

void Game::Destory()
{
	spdlog::info("app is destoryed");
}
