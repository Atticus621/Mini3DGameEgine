#include "game.h"

#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>
#include <GLES3/gl3.h>
#include <string>
#include "test_object.h"

void TestObject::Update(float delta)
{
    engine::GameObject::Update(delta);

	//auto position = GetPosition();
 //   auto& input = engine::Engine::GetInstance().GetInputManager();
 //   if (input.GetKeyPressed(GLFW_KEY_A)) {
	//	position.x -= 0.01;
 //   }
 //   if (input.GetKeyPressed(GLFW_KEY_D)) {
	//	position.x += 0.01;
 //   }
 //   if (input.GetKeyPressed(GLFW_KEY_W)) {
	//	position.y += 0.01;
 //   }
 //   if (input.GetKeyPressed(GLFW_KEY_S)) {
	//	position.y -= 0.01;
 //   }

	//SetPosition(position);


}



TestObject::TestObject()

{
    // Shader sources
    std::string vertexShaderSource = R"(#version 300 es
    layout (location = 0) in vec3 position;
    layout (location = 1) in vec3 color;

    out vec3 vColor;
    
    uniform mat4 uModel;
    uniform mat4 uView;
    uniform mat4 uProjection;

    void main() {

        vColor=color;
        gl_Position = uProjection * uView * uModel * vec4(position,1.0);
    }
)";

    std::string fragmentShaderSource = R"(#version 300 es
    precision mediump float;

    in vec3 vColor;
    
    out vec4 FragColor;

    void main() {
        FragColor = vec4(vColor, 1.0);
    }
)";

    auto& graphicAPI = engine::Engine::GetInstance().GetGraphicAPI();
    auto shaderProgram = graphicAPI.CreateShaderProgram(vertexShaderSource, fragmentShaderSource);

    spdlog::info("create shaderProgram :{}", static_cast<unsigned int>(shaderProgram->GetShaderProgramID()));

	auto material = std::make_shared<engine::Material>();
    material->SetShaderProgram(shaderProgram);

    std::vector<float> vertices{
         0.5f, 0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
        -0.5f,-0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
         0.5f,-0.5f, 0.0f,  0.0f, 0.0f, 1.0f
    };

    std::vector<uint32_t> indices{
        0,1,2,
        0,2,3
    };

    engine::VertexLayout vertexLayout;
    vertexLayout.elements.push_back(
        { 0,3,GL_FLOAT,0 }
    );
    vertexLayout.elements.push_back({ 1,3,GL_FLOAT,3 * sizeof(float) });
    vertexLayout.stride = 6 * sizeof(float);

    vertexLayout.logInfo();

    auto mesh = std::make_shared<engine::Mesh>(vertexLayout, vertices, indices);
    auto ptr = new engine::MeshComponent( material,mesh);
	AddComponent(ptr);
}
