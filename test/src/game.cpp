#include "game.h"
#include "test_object.h"

#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>
#include <GLES3/gl3.h>
#include <string>
#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif // !GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

bool Game::Init()
{
	auto& fs = engine::Engine::GetInstance().GetFileSystem();
	auto& grahphicAPI = engine::Engine::GetInstance().GetGraphicAPI();

	m_currentScene = new engine::Scene();
	engine::Engine::GetInstance().setCurrentScene(m_currentScene);
	auto camera = m_currentScene->CreateGameObject("MainCamera");
	auto cameraComponent = new engine::CameraComponent();
	camera->AddComponent(cameraComponent);
	camera->AddComponent(new engine::PlayerControlComponent());

	camera->SetPosition({ 0.0f,0.0f,1.0f });

	m_currentScene->SetMainCamera(camera);

	auto shaderProgram = grahphicAPI.GetDefaultShaderProgram();
	auto material = std::make_shared<engine::Material>();
	material->SetShaderProgram(shaderProgram);
	auto breakTexture = engine::Texture::load("textures\\break.jpg");
	material->SetParam("baseColorTexture", breakTexture);

	auto mesh = engine::Mesh::CreateCubeMesh();
    auto light = m_currentScene->CreateGameObject("Light");
    auto lightComponent = new engine::LightComponent();
    light->AddComponent(lightComponent);
    light->SetPosition({ 5.0f,5.0f,5.0f });

    auto cubeA = m_currentScene->CreateGameObject("CubeA");
	cubeA->AddComponent(new engine::MeshComponent(material, mesh));
	cubeA->SetPosition({ 0.5f,0.0f,0.0f });
	cubeA->SetScale({ 0.5f,0.5f,0.5f });

 //   auto cubeB = m_currentScene->CreateGameObject("CubeB");
 //   cubeB->AddComponent(new engine::MeshComponent(material, mesh));
 //   cubeB->SetPosition({ 0.0f,0.0f,0.0f });
 //   cubeB->SetScale({ 0.5f,0.5f,0.5f });

	//auto cubeC = m_currentScene->CreateGameObject("CubeC");
	//cubeC->AddComponent(new engine::MeshComponent(material, mesh));
	//cubeC->SetPosition({ -0.5f,0.0f,0.0f });
	//cubeC->SetScale({ 0.5f,0.5f,0.5f });

	//auto duckMesh = engine::Mesh::Load("models\\Duck.gltf");
	//auto duckMaterial = engine::Material::Load("materials\\duck.mat");
	//auto duck = m_currentScene->CreateGameObject("Duck");
	//duck->AddComponent(new engine::MeshComponent(duckMaterial, duckMesh));
	//duck->SetPosition({ 0.0f,0.0f,1.0f });
 //   
	
	auto duck = engine::GameObject::LoadGLTF("models\\Duck.gltf");
	duck->SetPosition({ -0.5f, 0.0f, 0.5f });
	duck->SetParent(camera);
	spdlog::info("position {},rotation{},scale {}", glm::to_string(duck->GetWorldPosition()), 
		glm::to_string(duck->GetRotation()),glm::to_string( duck->GetScale()));
	
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
