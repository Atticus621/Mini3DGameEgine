#include "game.h"
#include "test_object.h"

#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>
#include <GLES3/gl3.h>
#include <string>


bool Game::Init()
{
	auto& fs = engine::Engine::GetInstance().GetFileSystem();


	m_currentScene = new engine::Scene();
	auto camera = m_currentScene->CreateGameObject("MainCamera");
	auto cameraComponent = new engine::CameraComponent();
	camera->AddComponent(cameraComponent);
	camera->AddComponent(new engine::PlayerControlComponent());

	camera->SetPosition({ 0.0f,0.0f,2.0f });

	m_currentScene->SetMainCamera(camera);

    
    auto material = engine::Material::Load("materials\\break.mat");

    std::vector<float> vertices{
        // 正面 (front face)
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,

         // 顶面 (top face)
          0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
         -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
          0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,

          // 背面 (back face)
           0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
          -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
          -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
           0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,

           // 底面 (bottom face)
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
           -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
           -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,

            // 左面 (left face)
            -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,

            // 右面 (right face)
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f
    };

    std::vector<uint32_t> indices{
    0, 1, 2,  0, 2, 3,   // 正面
    4, 5, 6,  4, 6, 7,   // 顶面
    8, 9,10,  8,10,11,   // 背面
    12,13,14, 12,14,15,  // 底面
    16,17,18, 16,18,19,  // 左面
    20,21,22, 20,22,23   // 右面
    };

    engine::VertexLayout vertexLayout;
    vertexLayout.elements.push_back(
        { 0,3,GL_FLOAT,0 }
    );
    vertexLayout.elements.push_back({ 1,3,GL_FLOAT,3 * sizeof(float) });
	vertexLayout.elements.push_back({ 2,2,GL_FLOAT,6 * sizeof(float) });
    vertexLayout.stride = 8 * sizeof(float);

    vertexLayout.logInfo();

    auto mesh = std::make_shared<engine::Mesh>(vertexLayout, vertices, indices);

    auto cubeA = m_currentScene->CreateGameObject("CubeA");
	cubeA->AddComponent(new engine::MeshComponent(material, mesh));
	cubeA->SetPosition({ 0.5f,0.0f,0.0f });
	cubeA->SetScale({ 0.5f,0.5f,0.5f });

    auto cubeB = m_currentScene->CreateGameObject("CubeB");
    cubeB->AddComponent(new engine::MeshComponent(material, mesh));
    cubeB->SetPosition({ 0.0f,0.0f,0.0f });
    cubeB->SetScale({ 0.5f,0.5f,0.5f });

	auto cubeC = m_currentScene->CreateGameObject("CubeC");
	cubeC->AddComponent(new engine::MeshComponent(material, mesh));
	cubeC->SetPosition({ -0.5f,0.0f,0.0f });
	cubeC->SetScale({ 0.5f,0.5f,0.5f });

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
