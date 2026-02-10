#include "input_manager.h"
#include <spdlog/spdlog.h>

void engine::InputManager::SetKeyPressed(int key, bool pressed)
{
	if (key<0 || key>=m_keys.size()) {
		spdlog::error("{} is out of bound", key);
	}
	m_keys.at(key) = pressed;
}

bool engine::InputManager::GetKeyPressed(int key)
{
	if (key < 0 || key >= m_keys.size()) {
		spdlog::error("{} is out of bound", key);
	}
	return m_keys.at(key);
}

void engine::InputManager::SetMouseButtonPressed(int button, bool pressed)
{
	if (button < 0 || button >= m_mouseButtons.size()) {
		spdlog::error("{} is out of bound", button);
	}
	m_mouseButtons.at(button) = pressed;
}

bool engine::InputManager::GetMouseButtonPressed(int button)
{
	if (button < 0 || button >= m_mouseButtons.size()) {
		spdlog::error("{} is out of bound", button);
	}
	return m_mouseButtons.at(button);
}

void engine::InputManager::SetMouseOldPosition(float x, float y)
{
	m_mouseOldPosition = { x,y };
}

glm::vec2 engine::InputManager::GetMouseOldPosition() const
{
	return m_mouseOldPosition;
}

void engine::InputManager::SetMouseCurrentPosition(float x, float y)
{
	m_mouseCurrentPosition = { x,y };
}

glm::vec2 engine::InputManager::GetMouseCurrentPosition() const
{
	return m_mouseCurrentPosition;
}
