#pragma once
#include <array>

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
	private:
		friend class Engine;
		std::array<bool, 512> m_keys = { false };
	};
}