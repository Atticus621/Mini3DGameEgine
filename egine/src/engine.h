#pragma once

#include <memory>
#include <chrono>
#include <spdlog/spdlog.h>
#include "input/input_manager.h"
#include "graphic/graphic_api.h"
#include "render/render_queue.h"
#include "scene/scene.h"
#include "io/file_system.h"
#include "graphic/texture.h"


struct GLFWwindow;
namespace engine {

	class Application;
	class Engine {
	private:
		Engine() = default;
		Engine(const Engine&) = delete;
		Engine(Engine&&) = delete;
		Engine& operator=(const Engine&) = delete;
		Engine& operator=(Engine&&) = delete;
	public:
		static Engine& GetInstance();
		bool Init(int width,int height);
		void Run();
		void Destory();

		Application* GetAplication();
		void SetApplication(Application* application);
		InputManager& GetInputManager();
		GraphicAPI& GetGraphicAPI();
		RenderQueue& GetRenderQueue();
		FileSystem& GetFileSystem();
		TextureManager& GetTextureManager();
		Scene* GetCurrentScene() const { return m_currentScene.get(); }
		void setCurrentScene(Scene* scene) { m_currentScene.reset(scene); }
	private:
		std::unique_ptr<Application> m_application;
		std::chrono::steady_clock::time_point m_lastTimePoint;

		GLFWwindow* m_window = nullptr;
		InputManager m_inputManager;
		GraphicAPI m_graphicAPI;
		RenderQueue m_renderQueue;
		FileSystem m_fileSystem;
		TextureManager m_TextureManager;
		std::unique_ptr<Scene> m_currentScene;

	private:
		static void glfwErrorCallBack(int error, const char* description)
		{
			spdlog::error("GLFW Error({},{}", error, description);
		}
	};
}