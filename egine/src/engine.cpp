#define GLFW_INCLUDE_NONE

#include "engine.h"
#include "application.h"
#include "input/input_manager.h"

#include <windows.h>
#include <iostream>

#include <GLFW/glfw3.h>
#include <GLES3/gl3.h>
#include <spdlog/spdlog.h>


namespace engine {
	void glfwWinodwKeyCallBack(GLFWwindow* window, int key, int sacnCode, int action, int mods) {
		
		auto& inputManager = Engine::GetInstance().GetInputManager();
		if (action == GLFW_PRESS) {
			inputManager.SetKeyPressed(key, true);
		}
		else if (action == GLFW_RELEASE) {
			inputManager.SetKeyPressed(key, false);
		}
	}
}
engine::Engine& engine::Engine::GetInstance()
{
	static Engine engine;
	return engine;
}

bool engine::Engine::Init(int width,int height)
{
	if (!m_application) {

		return false;
	};


	glfwSetErrorCallback(&Engine::glfwErrorCallBack);

	if (!glfwInit()) {
		return false;
	}


	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	// Create window

	m_window = glfwCreateWindow(width, height, "3DGameEngine", nullptr, nullptr);
	if (m_window == nullptr) {
		spdlog::error("GLFW create window error");
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_window);

	glfwSetKeyCallback(m_window, glfwWinodwKeyCallBack);
	return m_application->Init();
}

void engine::Engine::Run()
{
	if (!m_application) {
		return;
	}

	m_lastTimePoint = std::chrono::high_resolution_clock::now();
	while (!glfwWindowShouldClose(m_window) && !m_application->GetNeedToBeClosed()) {
		glfwPollEvents();

		auto now= std::chrono::high_resolution_clock::now();
		float delta = std::chrono::duration<float>(now - m_lastTimePoint).count();
		m_lastTimePoint = now;

		m_application->Update(delta);

		m_graphicAPI.SetClearCorlor(1.0f, 1.0f, 1.0f, 1.0f);
		m_graphicAPI.ClearBuffer();

		m_renderQueue.Draw(m_graphicAPI);

		glfwSwapBuffers(m_window);

	}
}

void engine::Engine::Destory()
{
	if (!m_application) return;
	glfwTerminate();
	m_window = nullptr;
	m_application->Destory();
	m_application.reset();
}

engine::Application* engine::Engine::GetAplication() 
{
	return m_application.get();
}

void engine::Engine::SetApplication(Application* application)
{

	m_application.reset(application);
}

engine::InputManager& engine::Engine::GetInputManager()
{
	return m_inputManager;
}

engine::GraphicAPI& engine::Engine::GetGraphicAPI()
{
	return m_graphicAPI;
}

engine::RenderQueue& engine::Engine::GetRenderQueue()
{
	return m_renderQueue;
}

