#include "game.h"
#include "test_object.h"

#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>
#include <GLES3/gl3.h>
#include <string>


bool Game::Init()
{
	m_currentScene = new engine::Scene();
	auto camera = m_currentScene->CreateGameObject("MainCamera");
	auto cameraComponent = new engine::CameraComponent();
	camera->AddComponent(cameraComponent);

	camera->SetPosition({ 0.0f,1.0f,3.0f });

	m_currentScene->SetMainCamera(camera);

	m_currentScene->CreateGameObject<TestObject>("TestObject");
    
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
