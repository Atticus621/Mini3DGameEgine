#include "game.h"
#include "test_object.h"

#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>
#include <GLES3/gl3.h>
#include <string>


bool Game::Init()
{
	m_scene.CreateGameObject<TestObject>("TestObject");
    
    return true;
}

void Game::Update(float delta)
{
	
	m_scene.Update(delta);
}

void Game::Destory()
{
	spdlog::info("app is destoryed");
}
