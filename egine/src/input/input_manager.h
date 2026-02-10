#pragma once
#include <array>
#include <glm/vec2.hpp>

namespace engine {

	class InputManager {
	private:
		InputManager() = default;
		InputManager(const InputManager&) = delete;
		InputManager(InputManager&&) = delete;
		InputManager& operator=(const InputManager&) = delete;
		InputManager& operator=(InputManager&&) = delete;
	public:
		void SetKeyPressed(int key, bool pressed = true);
		bool GetKeyPressed(int key);

		void SetMouseButtonPressed(int button, bool pressed = true);
		bool GetMouseButtonPressed(int button);

		void SetMouseOldPosition(float x, float y);
		glm::vec2 GetMouseOldPosition() const;

		void SetMouseCurrentPosition(float x, float y);
		glm::vec2 GetMouseCurrentPosition() const;
	private:
		friend class Engine;
		std::array<bool, 512> m_keys = { false };
		std::array<bool, 32> m_mouseButtons = { false };
		
		glm::vec2 m_mouseOldPosition{ 0.0f,0.0f };
		glm::vec2 m_mouseCurrentPosition{ 0.0f,0.0f };
	};
}