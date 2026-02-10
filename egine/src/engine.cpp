#define GLFW_INCLUDE_NONE

#include "engine.h"
#include "application.h"
#include "input/input_manager.h"

#include <windows.h>
#include <iostream>

#include <GLFW/glfw3.h>
#include <GLES3/gl3.h>
#include <spdlog/spdlog.h>
#include "scene/components/camera_component.h"

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

	void glfwWindowMouseButtonCallBack(GLFWwindow* window, int button, int action, int mods) {
		auto& inputManager = Engine::GetInstance().GetInputManager();
		if (action == GLFW_PRESS) {
			inputManager.SetMouseButtonPressed(button, true);
		}
		else if (action == GLFW_RELEASE) {
			inputManager.SetMouseButtonPressed(button, false);
		}
	}

	void glfwWindowCursorPosCallBack(GLFWwindow* window, double xpos, double ypos) {
		auto& inputManager = Engine::GetInstance().GetInputManager();
		inputManager.SetMouseOldPosition(inputManager.GetMouseCurrentPosition().x, inputManager.GetMouseCurrentPosition().y);
		inputManager.SetMouseCurrentPosition(static_cast<float>(xpos), static_cast<float>(ypos));
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
	glfwSetMouseButtonCallback(m_window, glfwWindowMouseButtonCallBack);
	glfwSetCursorPosCallback(m_window, glfwWindowCursorPosCallBack);

	if (!m_graphicAPI.Init())
	{
		spdlog::error("GraphicAPI init failed");
		return false;
	}
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

		int windowWidth=0, windowHeight=0;
		glfwGetWindowSize(m_window, &windowWidth, &windowHeight);
		float aspect = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);

		CameraData cameraData;
		auto mainCamera = m_currentScene->GetMainCamera();
		 if (mainCamera) {
			 auto cameraComponent = mainCamera->GetComponent<CameraComponent>();
			 if (cameraComponent) {
				 cameraData.viewMat = cameraComponent->GetViewMatrix();
				 cameraData.projectionMat = cameraComponent->GetProjectionMatrix(aspect);
			 }
			 else {
				 spdlog::warn("mainCamera does not have CameraComponent");
			 }
			 
		 }
		m_renderQueue.Draw(m_graphicAPI, cameraData);

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

