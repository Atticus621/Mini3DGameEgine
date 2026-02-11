#include "game.h"
#include "test_object.h"

#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>
#include <GLES3/gl3.h>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool Game::Init()
{
	auto& fs = engine::Engine::GetInstance().GetFileSystem();
	auto path = fs.GetAssetsPath() /"break.jpg";

	int width = 0, height = 0, channels = 0;
	unsigned char* data = stbi_load(path.string().c_str(), &width, &height, &channels, 0);
    if (data) {
        spdlog::info("Image loaded successfully: {} ({}x{}, {} channels)", path.string(), width, height, channels);
        stbi_image_free(data);
    }
    else {
        spdlog::error("Failed to load image: {}", path.string());
	}
	m_currentScene = new engine::Scene();
	auto camera = m_currentScene->CreateGameObject("MainCamera");
	auto cameraComponent = new engine::CameraComponent();
	camera->AddComponent(cameraComponent);
	camera->AddComponent(new engine::PlayerControlComponent());

	camera->SetPosition({ 0.0f,0.0f,2.0f });

	m_currentScene->SetMainCamera(camera);

	m_currentScene->CreateGameObject<TestObject>("TestObject");
    
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
         0.5f, 0.5f, 0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f,  0.0f, 1.0f, 0.0f,
        -0.5f,-0.5f, 0.5f,  0.0f, 1.0f, 0.0f,
         0.5f,-0.5f, 0.5f,  0.0f, 0.0f, 1.0f,

         0.5f, 0.5f,-0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f,-0.5f,  0.0f, 1.0f, 0.0f,
        -0.5f,-0.5f,-0.5f,  0.0f, 1.0f, 0.0f,
         0.5f,-0.5f,-0.5f,  0.0f, 0.0f, 1.0f,
    };

    std::vector<uint32_t> indices{
        //front face
        0,1,2,
        0,2,3,
        //back face
        4,5,6,
        4,6,7,
        //left face
        1,5,6,
        1,6,2,
        //right face
        0,7,4,
        0,3,7,
		//top face
		0,4,5,
		0,5,1,
		//bottom face
		2,6,7,
		2,7,3,
    };

    engine::VertexLayout vertexLayout;
    vertexLayout.elements.push_back(
        { 0,3,GL_FLOAT,0 }
    );
    vertexLayout.elements.push_back({ 1,3,GL_FLOAT,3 * sizeof(float) });
    vertexLayout.stride = 6 * sizeof(float);

    vertexLayout.logInfo();

    auto mesh = std::make_shared<engine::Mesh>(vertexLayout, vertices, indices);
    auto ptr = new engine::MeshComponent(material, mesh);

    auto cubeA = m_currentScene->CreateGameObject("CubeA");
	cubeA->AddComponent(ptr);
	cubeA->SetPosition({ -1.0f,2.0f,0.0f });
	cubeA->SetScale({ 0.5f,0.5f,0.5f });

	engine::Engine::GetInstance().setCurrentScene(m_currentScene);
    return true;
}

void Game::Update(float delta)
{
	
	m_currentScene->Update(delta);
}

void Game::Destory()
{
	spdlog::info("app is destoryed");
}
